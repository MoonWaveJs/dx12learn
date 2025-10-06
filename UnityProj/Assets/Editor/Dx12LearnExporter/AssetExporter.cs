using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using Newtonsoft.Json;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;

namespace Editor.Dx12LearnExporter
{
    [Serializable]
    class DXVector3
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
    class DXVector2
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
    class DXVector4
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
    }
    
    [Serializable]
    class Entity
    {
        public string Name;
        public DXVector3 Position;
        public DXVector3 Scale;
        public DXVector3 Rotation;
    }
    [Serializable]
    class DxMesh
    {
        public DXVector3[] Position;
        public DXVector3[] Normal;
        public DxColor32[] Color;
        public DXVector2[] Texcoord0;
        public DXVector2[] Texcoord1;
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
        UInt32 SectionsNum;
        UInt32[] vertexNums;
        UInt32[] indicesNums;
    }


    public class AssetExporter
    {
        public static byte[] SerializeToBinary<T>(T obj)
        {
            using (var memoryStream = new MemoryStream())
            {
                var formatter = new BinaryFormatter();
                formatter.Serialize(memoryStream, obj);
                return memoryStream.ToArray();
            }
        }
        
        private static void ExportMeshAsset(MeshFilter filter,string meshPath)
        {
            string directory = Path.Combine(Paths.project, "..",Path.GetDirectoryName(meshPath));
            string savePath = Path.Combine(directory, Path.GetFileName(meshPath)+".dxMesh");
            Directory.CreateDirectory(directory);
            DxMesh mesh = new DxMesh();
            Vector3[] pos = filter.sharedMesh.vertices.ToArray();
            mesh.Position = pos.Select(p => new DXVector3(p.x,p.y,p.z)).ToArray();
            Vector3[] normal = filter.sharedMesh.normals.ToArray();
            mesh.Normal = normal.Select(p => new DXVector3(p.x,p.y,p.z)).ToArray();
            Vector2[] uv0 = filter.sharedMesh.uv.ToArray();
            Vector2[] uv1 = filter.sharedMesh.uv2.ToArray();
            mesh.Texcoord0 = uv0.Select(p => new DXVector2(p.x,p.y)).ToArray();
            mesh.Texcoord1 = uv1.Select(p => new DXVector2(p.x,p.y)).ToArray();
            var colors  = filter.sharedMesh.colors32.ToArray();
            mesh.Color = colors.Select(p => new DxColor32(p.r,p.g,p.b,p.a)).ToArray();
            File.WriteAllBytes(savePath, SerializeToBinary(mesh));
            
            filter.sharedMesh.GetSubMesh()
        }

        [MenuItem("Dx12Learn/导出场景")]
        private static void ExportScene()
        {
            List<StaticMeshEntity> staticMeshes = new List<StaticMeshEntity>(); 
            var meshes = GameObject.FindObjectsOfType<MeshFilter>(false);
            foreach (var mesh in meshes)
            {
                if (mesh.GetComponent<MeshRenderer>())
                {
                    StaticMeshEntity staticMeshEntity = new StaticMeshEntity();
                    staticMeshEntity.Name = mesh.gameObject.name;
                    staticMeshEntity.Position = mesh.transform.position;
                    staticMeshEntity.Scale = mesh.transform.lossyScale;
                    staticMeshEntity.Rotation = mesh.transform.rotation.eulerAngles;
                    staticMeshEntity.MeshPath = AssetDatabase.GetAssetPath(mesh.sharedMesh);
                    staticMeshes.Add(staticMeshEntity);
                    ExportMeshAsset(mesh,staticMeshEntity.MeshPath);
                }
            }

            Debug.Log(JsonConvert.SerializeObject(staticMeshes));
        }
    }
}
