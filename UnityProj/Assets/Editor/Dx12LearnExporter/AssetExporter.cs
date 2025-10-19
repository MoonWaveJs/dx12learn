using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using Newtonsoft.Json;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.Rendering;

namespace Editor.Dx12LearnExporter
{
    [Serializable]
    public class DXVector3
    {
        public float x;
        public float y;
        public float z;

        public DXVector3(float x,float y,float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static implicit operator DXVector3(Vector3 value)
        {
            return new DXVector3(value.x, value.y, value.z);
        }
    }    
    
    [Serializable]
    public class DXVector2
    {
        public float x;
        public float y;

        public DXVector2(float x,float y)
        {
            this.x = x;
            this.y = y;
        }

        public static implicit operator DXVector2(Vector2 value)
        {
            return new DXVector2(value.x, value.y);
        }
    }
    
    [Serializable]
    public class DXVector4
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public DXVector4(float x,float y,float z,float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public static implicit operator DXVector4(Vector3 value)
        {
            return new DXVector4(value.x, value.y, value.z,0);
        }
        
        public static implicit operator DXVector4(Vector4 value)
        {
            return new DXVector4(value.x, value.y, value.z,value.w);
        }
        
        public static implicit operator DXVector4(Quaternion value)
        {
            return new DXVector4(value.x, value.y, value.z,value.w);
        }
    }
    
    [Serializable]
    class Entity
    {
        public string Name;
        public DXVector3 Position;
        public DXVector3 Scale;
        public DXVector4 Rotation;
    }
    [Serializable]
    class DxMesh
    {
        public DXVector3[] Position;
        public DXVector3[] Normal;
        public DxColor32[] Color;
        public DXVector2[] Texcoord0;
        public DXVector2[] Texcoord1;
        
        public DxMeshInfo[] SubMeshInfo;
        public int[][] SubMeshIndices;
    }

    [Serializable]
    public struct DxColor32
    {
        public byte r;
        public byte g;
        public byte b;
        public byte a;

        public DxColor32(byte r, byte g, byte b, byte a)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }
    }
    
    [Serializable]
    class StaticMeshEntity : Entity
    {
        public string MeshPath;
    }

    [Serializable]
    public struct DxMeshInfo
    {
        //     Bounding box of vertices in local space.
        public DXVector3 center { get; set; }
        public DXVector3 extend { get; set; }

        //     Face topology of this sub-mesh.
        public int topology { get; set; }

        //     Starting point inside the whole Mesh index buffer where the face index data is
        //     found.
        public int indexStart { get; set; }

        //     Index count for this sub-mesh face data.
        public int indexCount { get; set; }

        //     Offset that is added to each value in the index buffer, to compute the final
        //     vertex index.
        public int baseVertex { get; set; }

        //     First vertex in the index buffer for this sub-mesh.
        public int firstVertex { get; set; }

        //     Number of vertices used by the index buffer of this sub-mesh.
        public int vertexCount { get; set; }
        public string name { get; set; }

        public static implicit operator DxMeshInfo(SubMeshDescriptor value)
        {
            DxMeshInfo dxMeshInfo = new DxMeshInfo();
            dxMeshInfo.extend = value.bounds.extents;
            dxMeshInfo.center = value.bounds.center;
            dxMeshInfo.topology = (int)value.topology;
            dxMeshInfo.indexStart = value.indexStart;
            dxMeshInfo.indexCount = value.indexCount;
            dxMeshInfo.baseVertex = value.baseVertex;
            dxMeshInfo.firstVertex = value.firstVertex;
            dxMeshInfo.vertexCount = value.vertexCount;
            return dxMeshInfo;
        }
    }

    [Serializable]
    public struct DxTex
    {
        public int width;
        public int height;
        public TextureFormat format;
    }

    public class AssetExporter
    {
        public static void SerializeToBinary<T>(T obj,string path)
        {
            string serializedData = JsonConvert.SerializeObject(obj, Formatting.Indented);
            // 保存数据到txt文件
            System.IO.File.WriteAllText(path, serializedData);
            // using (FileStream fileStream = new FileStream(path, FileMode.Create))
            // {
            //     BinaryFormatter bf = new BinaryFormatter();
            //     bf.Serialize(fileStream, obj);
            //     fileStream.Flush();
            // } 
        }
        
        private static void ExportMeshAsset(string meshPath)
        {
            string directory = Path.Combine(Paths.project, "..",Path.GetDirectoryName(meshPath));
            string dest = Path.Combine(Paths.project, "..",meshPath);
            string source = Path.Combine(Paths.project,meshPath);
            
            if (File.Exists(source))
            {
                Directory.CreateDirectory(directory);
                // File.Copy(source,dest,true);
            }
            var sharedMeshes = AssetDatabase.LoadAllAssetsAtPath(meshPath);
            foreach (var sharedMesh1 in sharedMeshes)
            {
                if (sharedMesh1 is Mesh sharedMesh)
                {
                    string savePath = Path.Combine(directory, Path.GetFileName(sharedMesh.name)+".dxMesh");
                    DxMesh mesh = new DxMesh();

                    Vector3[] pos = sharedMesh.vertices.ToArray();
                    mesh.Position = pos.Select(p => new DXVector3(p.x,p.y,p.z)).ToArray();
                    Vector3[] normal = sharedMesh.normals.ToArray();
                    mesh.Normal = normal.Select(p => new DXVector3(p.x,p.y,p.z)).ToArray();
                    Vector2[] uv0 = sharedMesh.uv.ToArray();
                    Vector2[] uv1 = sharedMesh.uv2.ToArray();
                    mesh.Texcoord0 = uv0.Select(p => new DXVector2(p.x,p.y)).ToArray();
                    mesh.Texcoord1 = uv1.Select(p => new DXVector2(p.x,p.y)).ToArray();
                    var colors  = sharedMesh.colors32.ToArray();
                    mesh.Color = colors.Select(p => new DxColor32(p.r,p.g,p.b,p.a)).ToArray();
                    
                    DxMeshInfo[] dxMeshs = new DxMeshInfo[sharedMesh.subMeshCount];
                    mesh.SubMeshIndices = new int[sharedMesh.subMeshCount][];
                    for(int i = 0; i <  sharedMesh.subMeshCount; i++) 
                    {
                        dxMeshs[i] = sharedMesh.GetSubMesh(i);
                        dxMeshs[i].name = sharedMesh.name + "_" + i;
                        var indices = sharedMesh.GetIndices(i,false);
                        mesh.SubMeshIndices[i] = indices;
                    }
                    mesh.SubMeshInfo = dxMeshs;
                    SerializeToBinary(mesh, savePath);
                }
            }
        }

        private static bool AddBytes(List<byte> bytes, Color32 data)
        {
            bytes.Add(data.r);
            bytes.Add(data.g);
            bytes.Add(data.b);
            bytes.Add(data.a);
            return true;
        }
        private static void ExportTextureAsset(Texture2D texture2D)
        {
            var texturePath= AssetDatabase.GetAssetPath(texture2D);
            string directory = Path.Combine(Paths.project, "..", Path.GetDirectoryName(texturePath));
            string source = Path.Combine(Paths.project, texturePath);

            if (File.Exists(source))
            {
                Directory.CreateDirectory(directory);
                // File.Copy(source, dest, true);
            }
            var pixels = texture2D.GetPixels32();
            
            List<byte> bytes = new  List<byte>(pixels.Length * 4);

            DxTex dxtex = new DxTex();
            dxtex.width = texture2D.width;
            dxtex.height = texture2D.height;
            dxtex.format = texture2D.format;
            string savePath = Path.Combine(directory, Path.GetFileName(texture2D.name)+".dxTexInfo");
            string savePath1 = Path.Combine(directory, Path.GetFileName(texture2D.name)+".dxTex");
            SerializeToBinary(dxtex, savePath);
            
            Array.ForEach(pixels, (Color32 data) => AddBytes(bytes, data));
            File.WriteAllBytes(savePath1,bytes.ToArray());


        }

        [MenuItem("Dx12Learn/导出场景")]
        private static void ExportScene()
        {
            List<StaticMeshEntity> staticMeshes = new List<StaticMeshEntity>(); 
            var meshes = GameObject.FindObjectsOfType<MeshFilter>(false);
            // var skinMeshRenders = GameObject.FindObjectsOfType<SkinnedMeshRenderer>(false);
            // var skinMeshes = skinMeshRenders.Select(skin => skin.sharedMesh).ToArray();
            //
            
            
            foreach (var mesh in meshes)
            {
                if (mesh.GetComponent<MeshRenderer>())
                {
                    if (mesh.gameObject.activeSelf)
                    {
                        var mats = mesh.GetComponent<MeshRenderer>().sharedMaterials;
                        foreach (var mat in mats)
                        {
                            var matTexs = mat.GetTexturePropertyNames();
                            var texs = matTexs.Select(p => mat.GetTexture(p)).ToArray();

                            foreach (var tex in texs)
                            {
                                bool isAssets = AssetDatabase.GetAssetPath(tex).StartsWith("Assets");
                                if (isAssets && tex is Texture2D texture2D)
                                {
                                    ExportTextureAsset(texture2D);
                                }
                            }
                            bool b = AssetDatabase.GetAssetPath(mesh.sharedMesh).StartsWith("Assets");
                            if (b)
                            {
                                StaticMeshEntity staticMeshEntity = new StaticMeshEntity();
                                staticMeshEntity.Name = mesh.gameObject.name;
                                staticMeshEntity.Position = mesh.transform.position;
                                staticMeshEntity.Scale = mesh.transform.lossyScale;
                            
                                Quaternion outsideRotation = mesh.transform.rotation;
                                // Quaternion outsideRotation = Quaternion.Euler(new Vector3(euler.y, euler.x, euler.z));
                                staticMeshEntity.Rotation = outsideRotation;
                                var fbxPath = AssetDatabase.GetAssetPath(mesh.sharedMesh);
                                string directory = Path.Combine(Path.GetDirectoryName(fbxPath));
                                string meshPath = Path.Combine(directory, Path.GetFileName((mesh.sharedMesh.name)+".dxMesh"));
                                staticMeshEntity.MeshPath = meshPath;
                            
                                staticMeshes.Add(staticMeshEntity);
                                ExportMeshAsset(AssetDatabase.GetAssetPath(mesh.sharedMesh));
                            }
                        }
                    }

                }
            }
            
            string serializedData = JsonConvert.SerializeObject(staticMeshes, Formatting.Indented);

            // 保存数据到txt文件
            System.IO.File.WriteAllText(Path.Combine(Paths.project, "..","Assets\\World\\SimpleScene.txt"), serializedData);
        }
    }
}
