using System;
using System.Collections.Generic;
using Newtonsoft.Json;
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

        DXVector3(float x,float y,float z)
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
    class Entity
    {
        public string Name;
        public DXVector3 Position;
        public DXVector3 Scale;
        public DXVector3 Rotation;
    }
    [Serializable]
    class StaticMeshEntity : Entity
    {
        public string MeshPath;
    }
    public class AssetExporter
    {
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
                }
            }

            Debug.Log(JsonConvert.SerializeObject(staticMeshes));
        }
    }
}
