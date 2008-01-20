/**
 * @file include/obj_loader.hpp
 *
 * @brief A simple loader for Wavefront .OBJ files
 *
 * @version 1.0
 * @author Andreas Weis
 *
 */
#ifndef __OBJ_LOADER_HPP_INCLUDE_GUARD__
#define __OBJ_LOADER_HPP_INCLUDE_GUARD__

#include <iostream>
#include <fstream>
#include <vector>
#include "../gbLib/include/gbException.hpp"

/** The mesh files generated by OBJ_FileLoader
 * @todo sophisticated parsing of face entries (e.g. vert//normal)
 * @note Note that the datasets for geometry, normals and texture coordinates
 *       are expected to have a size divisible by 3! Keep this in mind and
 *       ensure that you're only writing triples when using the respective
 *       Set*() and Add*() functions!
 */
class OBJ_Mesh {
public:
	/** A data type for storing per-poly information
	 */
	struct Face {
		int vert1, vert2, vert3;			///< indices into the vertex coordinate list m_geometry
		int normal1, normal2, normal3;		///< indices into the normal coordinate list m_normals
		int texture1, texture2, texture3;	///< indices into the texture coordinate list m_texcoords
		int smoothing_group;				///< an id specifying the face's smoothing group (-1 means undefined)
	};
private:
	std::vector<double> m_geometry;			///< a list of size (n_vertices*3) storing geometry data
	std::vector<double> m_normals;			///< a list of size (n_vertices*3) storing normal data
	std::vector<double> m_texcoords;		///< a list of size (???) storing texture coordinates
	std::vector<Face>   m_faces;			///< a list of size (n_triangles) storing all face data
	char*		        m_name;				///< the name of the mesh
public:
	/** Constructor
	 * @param[in] name Name of the mesh as null-terminated C-string (can be changed later invoking SetName() )
	 * @throw std::bad_alloc
	 */
	OBJ_Mesh(char const* name);
	/** Destructor
	 */
	~OBJ_Mesh();
	/** Copy Constructor
	 * @throw std::bad_alloc
	 */
	OBJ_Mesh(OBJ_Mesh const&);
	/** Change the name of the mesh
	 * @param[in] name Name of the mesh as null-terminated C-string
	 * @throw std::bad_alloc
	 */
	void SetName(char const* name);
	/** Replace the current geometry data
	 * @param[in] data Vector holding new geometry data
	 */
	void SetGeometry(std::vector<double> const& data);
	/** Replace the current geometry data
	 * @param[in] data Field holding new geometry data
	 * @param[in] n_data Size of field data
	 */
	template<typename T>
	void SetGeometry(T const* data, int n_data);
	/** Append new geometry data
	 * @param[in] data Field holding new geometry data
	 * @param[in] n_data Size of field data
	 */
	template<typename T>
	void AddGeometry(T const* data, int n_data);
	/** Replace the current normal data
	 * @param[in] data Vector holding new normal data
	 */
	void SetNormals(std::vector<double> const& data);
	/** Replace the current normal data
	 * @param[in] data Field holding new normal data
	 * @param[in] n_data Size of field data
	 */
	template<typename T>
	void SetNormals(T const* data, int n_data);
	/** Append new normal data
	 * @param[in] data Field holding new normal data
	 * @param[in] n_data Size of field data
	 */
	template<typename T>
	void AddNormals(T const* data, int n_data);
	/** Replace the current texture data
	 * @param[in] data Vector holding new texture data
	 */
	void SetTextureData(std::vector<double> const& data);
	/** Replace the current texture data
	 * @param[in] data Field holding new texture data
	 * @param[in] n_data Size of field data
	 */
	template<typename T>
	void SetTextureData(T const* data, int n_data);
	/** Append new texture data
	 * @param[in] data Field holding new texture data
	 * @param[in] n_data Size of field data
	 */
	template<typename T>
	void AddTextureData(T const* data, int n_data);
	/** Replace the current face data
	 * @param[in] data Vector holding new face data
	 */
	void SetFaceData(std::vector<Face> const& data);
	/** Replace the current face data
	 * @param[in] data Field holding new face data
	 * @param[in] n_data Size of field data
	 */
	void SetFaceData(Face const* data, int n_data);
	/** Append new face data
	 * @param[in] data Field holding new face data
	 * @param[in] n_data Size of field data
	 */
	void AddFaceData(Face const* data, int n_data);
	/** Delete all geometry data
	 */
	void ClearGeometry();
	/** Delete all normal data
	 */
	void ClearNormals();
	/** Delete all texture data
	 */
	void ClearTextureData();
	/** Delete all face data
	 */
	void ClearFaceData();
	/** Get the mesh name
	 * @return The name as null terminated C-string
	 */
	char const* GetName() const;
	/** Get the number of vertices currently in the mesh
	 * @return The number of vertices of the mesh
	 */
	int GetNVertices() const;
	/** Get the number of faces currently in the mesh
	 * @return The number of faces of the mesh
	 */
	int GetNFaces() const;
	/** Get the number of normals currently in the mesh
	 * @return The number of normals of the mesh
	 */
	int GetNNormals() const;
	/** Get the number of texture coordinates currently in the mesh
	 * @return The number of texture coordinates of the mesh
	 */
	int GetNTexture() const;
	/** Get an immediate (unindexed) representation of the mesh
	 * @param[out] mesh_geometry Pointer to a field of at least size (n_triangles*9) or NULL
	 * @param[out] mesh_normals Pointer to a field of at least size (n_triangles*9) or NULL
	 * @param[out] mesh_texture Pointer to a field of at least size (n_triangles*9) or NULL
	 * @param[in] scale A scale factor applied to each vertex
	 */
	template<typename T>
	void GetMeshGeometryUnindexed(T* mesh_geometry, T* mesh_normals, T* mesh_texture, T scale) const;
	/** Get a raw (indexed) representation of the mesh
	 * @param[out] mesh_geometry Pointer to a field of at least size (n_vertices*3)
	 * @param[out] mesh_normals Pointer to a field of at least size (n_normals*3)
	 * @param[out] mesh_texture Pointer to a field of at least size (n_texture*3)
	 * @param[out] mesh_faces Pointer to a field of at least size (n_faces)
	 * @param[in] scale A scale factor applied to each vertex
	 */
	template<typename T>
	void GetMeshGeometry(T* mesh_geometry, T* mesh_normals, T* mesh_texture, Face* mesh_faces, T scale) const;
	/** Get the X-coordinate of a vertex
	 * @param index Vertex index (0..n_vertices-1)
	 * @return The X coordinate of the vertex
	 */
	double const* GetVertexX(int index) const;
	/** Get the Y-coordinate of a vertex
	 * @param index Vertex index (0..n_vertices-1)
	 * @return The Y coordinate of the vertex
	 */
	double const* GetVertexY(int index) const;
	/** Get the Z-coordinate of a vertex
	 * @param index Vertex index (0..n_vertices-1)
	 * @return The Y coordinate of the vertex
	 */
	double const* GetVertexZ(int index) const;
	/** Get the X-coordinate of a normal vector
	 * @param index Normal vector index (0..n_normals-1)
	 * @return The X coordinate of the normal vector
	 */
	double const* GetNormalX(int index) const;
	/** Get the Y-coordinate of a normal vector
	 * @param index Normal vector index (0..n_normals-1)
	 * @return The Y coordinate of the normal vector
	 */
	double const* GetNormalY(int index) const;
	/** Get the Z-coordinate of a normal vector
	 * @param index Normal vector index (0..n_normals-1)
	 * @return The Z coordinate of the normal vector
	 */
	double const* GetNormalZ(int index) const;
	/** Get the X-coordinate (U) of a texture coordinate
	 * @param index Texture index (0..n_texture-1)
	 * @return The X texture coordinate (U)
	 */
	double const* GetTextureX(int index) const;
	/** Get the X-coordinate (V) of a texture coordinate
	 * @param index Texture index (0..n_texture-1)
	 * @return The Y texture coordinate (V)
	 */
	double const* GetTextureY(int index) const;
	/** Get the Z-coordinate (W) of a texture coordinate
	 * @param index Texture index (0..n_texture-1)
	 * @return The Z texture coordinate (W)
	 */
	double const* GetTextureZ(int index) const;
	/** Get the face indices for a specific face
	 * @param index Face index (0..n_faces-1)
	 * @return A Face structure containing all index information
	 */
	Face const* GetFace(int index) const;
private:
	OBJ_Mesh& operator=(OBJ_Mesh const&);			///< private copy assignment operator (not implemented)
};

#include "../src/obj_loader.impl.hpp"

/** A loader for Wavefront .obj-files
 */
class OBJ_FileLoader {
private:
	std::vector<OBJ_Mesh*> m_MeshList;				///< list of meshes found in the file
public:
	/** Constructor
	 */
	OBJ_FileLoader();
	/** Constructor
	 * @param[in] fname Full path to the file that shall be loaded
	 * @throw std::bad_alloc
	 */
	OBJ_FileLoader(const char* fname);
	/** Destructor
	 */
	~OBJ_FileLoader();
	/** Get the number of meshes found in the file
	 * @return The number of meshes found in the file
	 */
	int GetNMeshes() const;
	/** Get a Mesh from the file
	 * @param[in] index Number of the mesh from range [0..( GetNMeshes() - 1 )]
	 * @return A @ref OBJ_Mesh object of the mesh
	 */
	OBJ_Mesh const* GetMesh(int index) const;
	/** Add a mesh to the meshlist
	 * @param[in] mesh The mesh to add
	 * @throw std::bad_alloc
	 */
	void AddMesh(OBJ_Mesh const& mesh);
	/** Write data back to a file
	 * @param[in] fname Full path to the output file
	 * @throw Ghulbus::gbException GB_FAILED indicates a file access error
	 */
	void WriteFile(char const* fname) const;
private:
	/** Private helper function that does the actual parsing
	 * @throw Ghulbus::gbException GB_INVALIDCONTEXT indicates that the function was called while there
	 *                             where already objects in the meshlist
	 * @throw std::bad_alloc
	 */
	void ReadFile(std::ifstream& f);
};

#endif