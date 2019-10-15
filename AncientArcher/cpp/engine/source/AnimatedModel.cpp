#include <AnimatedModel.h>
#include <TextureLoader.h>
#include <iostream>

AnimatedModel::AnimatedModel(const std::string& path)
{
  Model::loadModel(path);
}

void AnimatedModel::update(float deltaTime, Shader* shader)
{

}

void AnimatedModel::render(Shader* shader, Camera* camera)
{
  Model::render(shader, camera);
}

//void AnimatedModel::loadModel(const std::string& path)
//{
//  Assimp::Importer assimpImporter;
//
//  assimpImporter.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, MAX_BONE_WEIGHTS);  // default is 4
//
//  const aiScene* scene = assimpImporter.ReadFile(path,
//    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
//    aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights);
//
//  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
//  {
//    std::cout << "ERROR::ASSIMP:: " << assimpImporter.GetErrorString() << std::endl;
//
//    throw(-1);
//    return;
//  }
//
//  _modelDir = path.substr(0, path.find_last_of('/'));
//  _globalInverseTransform = inverse(aiMat4_to_glmMat4(scene->mRootNode->mTransformation));  // save inverse root transformation
//
//
//  processNode(scene->mRootNode, scene);
//
//  processAnimations(scene);
//}

//void AnimatedModel::processNode(aiNode* node, const aiScene* scene)
//{
//  for (unsigned int i = 0; i < node->mNumMeshes; i++)
//  {
//    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//    _meshes.push_back(processMesh(mesh, scene));
//  }
//
//  for (unsigned int i = 0; i < node->mNumChildren; i++)
//  {
//    processNode(node->mChildren[i], scene);
//  }
//}

//Mesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//  // data to fill
//  std::vector<Vertex> vertices;
//  std::vector<unsigned int> indices;
//  std::vector<Texture> textures;
//  std::vector<BoneInfo> bones;
//
//  // Walk through each of the mesh's vertices
//  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//  {
//    Vertex tmp_vertex;
//    glm::vec3 tmp_vec3; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//    // positions
//    tmp_vec3 = aiVec3_to_glmVec3(mesh->mVertices[i]);
//    tmp_vertex.Position = tmp_vec3;
//    // normals
//    tmp_vec3 = aiVec3_to_glmVec3(mesh->mNormals[i]);
//
//    tmp_vertex.Normal = tmp_vec3;
//    // texture coordinates
//    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//    {
//      glm::vec2 vec;
//      // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
//      // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//      vec.x = mesh->mTextureCoords[0][i].x;
//      vec.y = mesh->mTextureCoords[0][i].y;
//      tmp_vertex.TexCoords = vec;
//    }
//    else
//    {
//      tmp_vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//    }
//    // tangent
//    tmp_vec3 = aiVec3_to_glmVec3(mesh->mTangents[i]);
//    tmp_vertex.Tangent = tmp_vec3;
//    // bitangent
//    tmp_vec3 = aiVec3_to_glmVec3(mesh->mBitangents[i]);
//    tmp_vertex.Bitangent = tmp_vec3;
//
//    vertices.push_back(tmp_vertex);
//  }
//
//  //std::ofstream debugBoneFile;
//  //debugBoneFile.open("debugBoneFile.txt");
//  for (unsigned int i = 0; i < mesh->mNumBones; i++)     // update boneMap for this mesh
//  {
//    unsigned int boneIndex = 0;
//    std::string boneName = mesh->mBones[i]->mName.C_Str();
//    if (_boneMap.find(boneName) == _boneMap.end())  // bone not found, add it
//    {
//      boneIndex = _numberOfBones;
//      _numberOfBones++;
//      BoneInfo bone;
//      bones.push_back(bone);
//    }
//    else // bone found, use found bone
//    {
//      boneIndex = _boneMap[boneName];
//    }
//
//    _boneMap[boneName] = boneIndex;
//    bones[boneIndex].BoneOffset = aiMat4_to_glmMat4(mesh->mBones[i]->mOffsetMatrix);
//
//    for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
//    {
//      unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;  // which vertex in the mesh
//      float weight = mesh->mBones[i]->mWeights[j].mWeight;             // weight towards that vertex in mesh
//      for (unsigned int k = 0; k < MAX_BONE_WEIGHTS; ++k)
//      {
//        if (vertices[vertexId].BoneWeight[k] <= 0.000001f)
//        {
//          vertices[vertexId].BoneId[k] = boneIndex;
//          vertices[vertexId].BoneWeight[k] = weight;
//          //debugBoneFile << "added boneId at: " << k << ", to vertex: " << vertexId << ", weight: " << weight << std::endl;
//          break;
//        }
//      }
//    }
//  }
//  //debugBoneFile.close();
//
//  // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//  {
//    aiFace face = mesh->mFaces[i];
//    // retrieve all indices of the face and store them in the indices vector
//    for (unsigned int j = 0; j < face.mNumIndices; j++)
//    {
//      indices.push_back(face.mIndices[j]);
//    }
//  }
//  // process materials
//  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//  // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//  // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
//  // Same applies to other texture as the following list summarizes:
//  // diffuse: texture_diffuseN
//  // specular: texture_specularN
//  // normal: texture_normalN
//
//  // 1. diffuse maps
//  std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//  // 2. specular maps
//  std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//  // 3. normal maps
//  std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//  // 4. height maps
//  std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//  // return a mesh object created from the extracted mesh data
//  return Mesh(vertices, indices, textures, bones);
//}

void AnimatedModel::processAnimations(const aiScene* scene)
{
  if (scene->mNumAnimations == 0)
  {
    throw(-6);
  }

  for (unsigned int i = 0; i < scene->mNumAnimations; i++)
  {
    Animation anim;
    anim.Duration = scene->mAnimations[i]->mDuration;
    anim.TicksPerSeconds = scene->mAnimations[i]->mTicksPerSecond;
    _animations.push_back(anim);

    //save all channel data for each channel
    for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
    {

      Channel tmp_channel;
      tmp_channel.Name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();    // save name: .C_Str() or .data -- which is better?

      // save positions with their timings
      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)  // save positions
      {
        TimedVec3 tmp_pos;
        tmp_pos._time = (float)scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
        tmp_pos._data = aiVec3_to_glmVec3(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue);
        tmp_channel.Positions.push_back(tmp_pos);
      }
      // save rotations with their timings
      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)  // save rotations
      {
        TimedQuat tmp_quaternion;
        tmp_quaternion._time = (float)scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
        tmp_quaternion._data = aiQuat_to_glmQuat(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue);
        tmp_channel.Rotations.push_back(tmp_quaternion);
      }
      // save scaling with their timings
      for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)  // save scalings
      {
        TimedVec3 tmp_scale;
        tmp_scale._time = (float)scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
        tmp_scale._data = aiVec3_to_glmVec3(scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue);
        tmp_channel.Scalings.push_back(tmp_scale);
      }

      _animations[i].Channels.push_back(tmp_channel);
    }
  }
}

void AnimatedModel::loadBones(unsigned int MeshIndex, const aiMesh* mesh)
{

}

void AnimatedModel::readNodeHeirarchy(float animationTime, glm::mat4 parentTransformation, int meshNum, int animNum, int localBranchNum)
{
  std::string nodeName;
  glm::mat4 nodeTransformation;
  bool isNameFound = false;
  int channelNumber = 0;

  for (auto i = 0; i < _animations[animNum].Channels.size(); i++)
  {
    //const aiNodeAnim* nA = animation->mChannels[i];
    if (nodeName.compare(_animations[animNum].Channels[i].Name) == 0)  //name found in nodes
    {
      isNameFound = true;
      channelNumber = i;
      break;
    }
  }

  if (isNameFound)
  {
    // calc scaling
    glm::vec3 scaling(0.f);
    glm::mat4 scalingMat4(1.f);
    std::size_t scalingSize = _animations[animNum].Channels[channelNumber].Scalings.size();
    if (scalingSize == 1)
    {
      scaling = _animations[animNum].Channels[channelNumber].Scalings[0]._data;
    }
    else
    {
      unsigned int scalingIndex = 0;
      assert(scalingSize > 1);
      for (unsigned int i = 0; i < scalingSize - 1; i++)  // should this be scalingSize - 1?
      {
        if (animationTime < (float)_animations[animNum].Channels[channelNumber].Scalings[i + 1]._time)
        {
          scalingIndex = i;
          break;
        }
      }
      unsigned int nextScalingIndex = scalingIndex + 1;
      float deltaTime =
        (float)(_animations[animNum].Channels[channelNumber].Scalings[nextScalingIndex]._time - _animations[animNum].Channels[channelNumber].Scalings[scalingIndex]._time);
      float factor = (animationTime - (float)(_animations[animNum].Channels[channelNumber].Scalings[scalingIndex]._time)) / deltaTime;
      glm::vec3 start, end;
      start = _animations[animNum].Channels[channelNumber].Scalings[scalingIndex]._data;
      end = _animations[animNum].Channels[channelNumber].Scalings[nextScalingIndex]._data;
      glm::vec3 vec3Delta = end - start;
      scaling = start + factor * vec3Delta;
      scalingMat4 = scale(scalingMat4, scaling);
    }

    // calc interpolation rotation and gen rotation transform matrix
    glm::quat rotationQ;
    glm::mat4 rotationMat4;
    std::size_t rotationSize = _animations[animNum].Channels[channelNumber].Rotations.size();
    if (rotationSize == 1)
    {
      rotationQ = _animations[animNum].Channels[channelNumber].Rotations[0]._data;
    }
    else
    {
      unsigned int rotationIndex = 0;
      for (unsigned int i = 0; i < rotationSize - 1; i++)
      {
        if (animationTime < (float)_animations[animNum].Channels[channelNumber].Rotations[i + 1]._time)
        {
          rotationIndex = i;
          break;
        }
      }

      unsigned int nextRotIndex = (rotationIndex + 1);
      float deltaTime = (float)(_animations[animNum].Channels[channelNumber].Rotations[nextRotIndex]._time - _animations[animNum].Channels[channelNumber].Rotations[rotationIndex]._time);
      float factor = (animationTime - (float)(_animations[animNum].Channels[channelNumber].Rotations[rotationIndex]._time)) / deltaTime;
      glm::quat start, end;
      start = _animations[animNum].Channels[channelNumber].Rotations[rotationIndex]._data;
      start = _animations[animNum].Channels[channelNumber].Rotations[nextRotIndex]._data;
      rotationQ = normalize(mix(start, end, factor));
      rotationMat4 = mat4_cast(rotationQ);
    }

    // interpolate position and generate translation transform matrix
    glm::vec3 translation;
    glm::mat4 translationMat4(1.f);
    std::size_t translateSize = _animations[animNum].Channels[channelNumber].Positions.size();
    if (translateSize == 1)
    {
      translation = _animations[animNum].Channels[channelNumber].Positions[0]._data;
    }
    else
    {
      unsigned int positionIndex = 0;
      for (unsigned int i = 0; i < translateSize - 1; i++)
      {
        if (animationTime < (float)_animations[animNum].Channels[channelNumber].Positions[i + 1]._time)
        {
          positionIndex = i;
          break;
        }
      }
      unsigned int nextPosIndex = positionIndex + 1;
      float deltaTime = (float)(_animations[animNum].Channels[channelNumber].Positions[nextPosIndex]._time - _animations[animNum].Channels[channelNumber].Positions[positionIndex]._time);
      float factor = (animationTime - (float)(_animations[animNum].Channels[channelNumber].Positions[positionIndex]._time)) / deltaTime;
      glm::vec3 start, end;
      start = _animations[animNum].Channels[channelNumber].Positions[positionIndex]._data;
      end = _animations[animNum].Channels[channelNumber].Positions[nextPosIndex]._data;
      glm::vec3 delta = end - start;
      translation = start + factor * delta;
      translationMat4 = translate(translationMat4, translation);
    }

    // combine all translations together
    nodeTransformation = translationMat4 * rotationMat4 * scalingMat4;
  }

  // combine with parent to get global space transform
  glm::mat4 globalTransforamtion = parentTransformation * nodeTransformation;

  if (_boneMap.find(nodeName) != _boneMap.end())
  {
    auto boneIndex = _boneMap[nodeName];
    _meshes[meshNum].boneInfo[boneIndex].FinalTransformation = _globalInverseTransform * globalTransforamtion * _meshes[meshNum].boneInfo[boneIndex].BoneOffset;
  }

  for (auto i = 0; i < _nodes.size(); i++)
  {
    readNodeHeirarchy(animationTime, globalTransforamtion, meshNum, animNum, localBranchNum);
  }

}

glm::mat4 aiMat4_to_glmMat4(const aiMatrix4x4& inMat)
{
  glm::mat4 outMat;
  outMat[0][0] = inMat.a1;
  outMat[1][0] = inMat.b1;
  outMat[2][0] = inMat.c1;
  outMat[3][0] = inMat.d1;
  outMat[0][1] = inMat.a2;
  outMat[1][1] = inMat.b2;
  outMat[2][1] = inMat.c2;
  outMat[3][1] = inMat.d2;
  outMat[0][2] = inMat.a3;
  outMat[1][2] = inMat.b3;
  outMat[2][2] = inMat.c3;
  outMat[3][2] = inMat.d3;
  outMat[0][3] = inMat.a4;
  outMat[1][3] = inMat.b4;
  outMat[2][3] = inMat.c4;
  outMat[3][3] = inMat.d4;
  return outMat;
}

glm::vec3 aiVec3_to_glmVec3(const aiVector3D& inVec)
{
  glm::vec3 outVec;
  outVec.x = inVec.x;
  outVec.y = inVec.y;
  outVec.z = inVec.z;
  return outVec;
}

glm::quat aiQuat_to_glmQuat(const aiQuaternion& inQuat)
{
  glm::quat outQuat;
  outQuat.w = inQuat.w;
  outQuat.x = inQuat.x;
  outQuat.y = inQuat.y;
  outQuat.z = inQuat.z;
  return outQuat;
}

glm::vec3 calcInterpolatedScaling(glm::vec3 inVec, float animTime, aiNodeAnim animNode)
{
  return glm::vec3();
}

glm::quat calcInterpolatedRotation(glm::quat inQuat, float animTime, aiNodeAnim animNode)
{
  return glm::quat();
}

glm::vec3 calcInterpolatedPosition(glm::vec3 inVec, float animTime, aiNodeAnim animNode)
{
  return glm::vec3();
}
