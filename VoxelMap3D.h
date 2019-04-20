#pragma once

class Model3D;
class Face3D;
class Triangle3D;

#include "VoxelMaterial3D.h"
#include <vector>

static bool compz(Vec3f& a, Vec3f& b) { return a.z < b.z; }
static bool compy(Vec3f& a, Vec3f& b) { return a.y < b.y; }

#define SIGN(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

class VoxelMap3D
{
	int *voxelMapPtr = 0;
	Model3D* model = 0;
	Vec3i size;
	int model2RamScale;
	int GetVoxelIndex(Vec3i& position);
	void SetVoxelUnsafe(Vec3i& position, VoxelMaterial3D& material); // pozisyonun harita sýnýrlarýnda olup olmadýðý model deðiþkenindeki sýnýrlardan dolayý kontrol edilmiyor. okunacak model dosyasýna exploit yoluyla kod yerleþtirilebilmesi kontrol edilmedi (unsafe).
	VoxelMaterial3D* GetVoxelUnsafe(Vec3i& position);
	std::vector<Vec3f> voxelPoints;
	std::vector<VoxelMaterial3D> voxelMaterials;
	void FillTriangle(Triangle3D& triangle, VoxelMaterial3D& material);
	void FillDividedTriangle(Vec3f& vPeak, Vec3f& v1, Vec3f& v2, VoxelMaterial3D& material);
	void DrawLine(Vec3f p1, Vec3f p2, VoxelMaterial3D& material);
	void FillTriangle4Normal(Triangle3D& triangle, VoxelMaterial3D& material);
	bool EdgeVoxelTest(Vec3f& voxelCenter, Triangle3D& triangle);
	void PrepareDistanceMapping();
public:
	unsigned long long castCounter = 0;
	VoxelMap3D(Model3D* _model);
	Vec3i GetSize() { return size; }
	Model3D* GetModel() { return model; }
	VoxelMaterial3D* CastRay(Vec3f& voxelRayOrigin, Vec3f& voxelRayDirection);
	VoxelMaterial3D* MarchRay(Vec3f& voxelRayOrigin, Vec3f& voxelRayDirection);
	int GetModel2RamScale() { return model2RamScale; }
};

/*
0 0 0 0 0
0 A 0 0 0
0 0 B 0 0
->voronoi diyagramý zorluðundan dolayý ray marching'den önce ray casting uygulanacak. A ve B deðerleri, voxelMaterials dizisinin indislerinin 1 fazlasýdýr.
->Ýndis yerine adres kullanýlmamasýnýn sebebi haritayý 2 bytelik elementlerden de oluþturabilmek çünkü çok fazla harita olursa ram sýkýntýsý olabilir ancak sabit bir þekilde 2 byte kullanýlmasý diðer yönteme göre fazladan 1 veri okumasý gerçekleþireceði için raylarýn çarptýðý toplam alan kadar fazladan veri okunur.
->harita 2 bytelik elementlerden oluþursa, voxelMaterials dizisinin boyutu iþaretli short tipi deðiþken boyutunu(+32,767) aþabilir.
->(*)kýsacasý voxelMapPtr iþaretçisinin tipi 4 byte olursa tek dezavantaj ram'de haritalarýn fazla yer kaplamasý.
*/

/*
this->size = (model->size * scale);
int voxelMapPtrSize = (this->size + Vec3i(1,1,1)).Volume(); // harita üzerindeki maksimum noktalar da kullanýlacaðý için dizi her eksende 1 geniþletiliyor.
*/

/*
ray marching için, voxel haritasýnýn elementi pozitifse, rayýn ilerleyeceði birim; negatifse, material indisi
*/

/*
voxellerin kapladýðý toplam birim alan kadar VoxelMaterial3D elementi olacak. çünkü çok yüksek çözünürlükte ayný face üzerinde farklýlýklar olur. 
*/