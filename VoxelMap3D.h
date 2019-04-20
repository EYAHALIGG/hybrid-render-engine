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
	void SetVoxelUnsafe(Vec3i& position, VoxelMaterial3D& material); // pozisyonun harita s�n�rlar�nda olup olmad��� model de�i�kenindeki s�n�rlardan dolay� kontrol edilmiyor. okunacak model dosyas�na exploit yoluyla kod yerle�tirilebilmesi kontrol edilmedi (unsafe).
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
->voronoi diyagram� zorlu�undan dolay� ray marching'den �nce ray casting uygulanacak. A ve B de�erleri, voxelMaterials dizisinin indislerinin 1 fazlas�d�r.
->�ndis yerine adres kullan�lmamas�n�n sebebi haritay� 2 bytelik elementlerden de olu�turabilmek ��nk� �ok fazla harita olursa ram s�k�nt�s� olabilir ancak sabit bir �ekilde 2 byte kullan�lmas� di�er y�nteme g�re fazladan 1 veri okumas� ger�ekle�irece�i i�in raylar�n �arpt��� toplam alan kadar fazladan veri okunur.
->harita 2 bytelik elementlerden olu�ursa, voxelMaterials dizisinin boyutu i�aretli short tipi de�i�ken boyutunu(+32,767) a�abilir.
->(*)k�sacas� voxelMapPtr i�aret�isinin tipi 4 byte olursa tek dezavantaj ram'de haritalar�n fazla yer kaplamas�.
*/

/*
this->size = (model->size * scale);
int voxelMapPtrSize = (this->size + Vec3i(1,1,1)).Volume(); // harita �zerindeki maksimum noktalar da kullan�laca�� i�in dizi her eksende 1 geni�letiliyor.
*/

/*
ray marching i�in, voxel haritas�n�n elementi pozitifse, ray�n ilerleyece�i birim; negatifse, material indisi
*/

/*
voxellerin kaplad��� toplam birim alan kadar VoxelMaterial3D elementi olacak. ��nk� �ok y�ksek ��z�n�rl�kte ayn� face �zerinde farkl�l�klar olur. 
*/