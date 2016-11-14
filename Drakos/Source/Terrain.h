#pragma once

#include <d3d11_2.h>
#include "TerrainCell.h"
#include "Frustum.h"

class Terrain
{

#pragma pack(2) 
	typedef struct
	{
		unsigned short bfType;
		unsigned int   bfSize;
		unsigned short bfReserved1;
		unsigned short bfReserved2;
		unsigned int   bfOffBits;
	} BITMAPFILEHEADER;
#pragma pack()

	typedef struct
	{
		unsigned int   biSize;
		int            biWidth;
		int            biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned int   biCompression;
		unsigned int   biSizeImage;
		int            biXPelsPerMeter;
		int            biYPelsPerMeter;
		unsigned int   biClrUsed;
		unsigned int   biClrImportant;
	} BITMAPINFOHEADER;

	struct HeightMapType
	{
		float x;
		float y;
		float z;
		float nx;
		float ny;
		float nz;
		float r;
		float g;
		float b;
	};

	struct ModelType
	{
		float x;
		float y;
		float z;
		float tu;
		float tv;
		float nx;
		float ny;
		float nz;
		float tx;
		float ty;
		float tz;
		float bx;
		float by;
		float bz;
		float r;
		float g;
		float b;
		float tu2;
		float tv2;
	};

	struct VectorType
	{
		float x;
		float y;
		float z;
	};

	struct TempVertexType
	{
		float x;
		float y;
		float z;
		float tu;
		float tv;
		float nx;
		float ny;
		float nz;
	};

public:
	Terrain();	
	~Terrain();

	bool Initialize(ID3D11Device*, char*);
	void Frame();
	bool RenderCell(ID3D11DeviceContext*, int, Frustum*);
	void RenderCellLines(ID3D11DeviceContext*, int) const;
	int GetCellIndexCount(int) const;
	int GetCellLinesIndexCount(int) const;
	int GetCellCount() const;
	int GetRenderCount() const;
	int GetCellsDrawn() const;
	int GetCellsCulled() const;
	bool GetHeightAtPosition(float, float, float&) const;

private:
	Terrain(const Terrain&);

	bool LoadSetupFile(char*);
	void ShutdownHeightMap();
	void SetTerrainCoordinates() const;
	bool CalculateNormals() const;
	bool LoadColorMap() const;
	bool BuildTerrainModel();
	void ShutdownTerrainModel();
	void CalculateTerrainVectors() const;
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&) const;
	bool LoadRawHeightMap();
	bool LoadTerrainCells(ID3D11Device*);
	void ShutdownTerrainCells();
	bool CheckHeightOfTriangle(float, float, float&, float[3], float[3], float[3]) const;

	int m_terrainHeight;
	int m_terrainWidth;
	int m_vertexCount;
	float m_heightScale;
	char *m_terrainFilename;
	char *m_colorMapFilename;
	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;
	TerrainCell* m_TerrainCells;
	int m_cellCount;
	int m_renderCount;
	int m_cellsDrawn;
	int m_cellsCulled;
};
