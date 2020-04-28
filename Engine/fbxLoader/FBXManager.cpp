/*
FBXManager
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "FBXManager.h"

FBXManager::FBXManager()
{
  mSdkManager = FbxManager::Create();
  mIos = FbxIOSettings::Create(mSdkManager, IOSROOT);
  mSdkManager->SetIOSettings(mIos);
}

FBXManager::~FBXManager()
{
  mSdkManager->Destroy();
  mScenes.clear();
}

bool FBXManager::LoadFBX(const char* filename)
{
  mImporter = FbxImporter::Create(mSdkManager, "");

  if (!mImporter->Initialize(filename, -1, mSdkManager->GetIOSettings()))
  {
    printf("call to fbximproter::initialize() failed.\n");
    printf("error returend; %s\n\n", mImporter->GetStatus().GetErrorString());
    mImporter->Destroy();
    return false;
  }

  FbxScene* lScene = FbxScene::Create(mSdkManager, "myScene");
  mImporter->Import(lScene);

  mScenes.push_back(lScene);

  // print the nodes of the scene and their attributes recursively.
  // note that we are not printing the root node becasue it should not contain any attributes.

  FbxNode* lRootNode = mScenes.back()->GetRootNode();
  if (lRootNode)
  {
    for (int i = 0; i < lRootNode->GetChildCount(); i++)
    {
      PrintNode(lRootNode->GetChild(i));
    }
  }

  mImporter->Destroy();

}

void FBXManager::PrintTabs()
{
  for (int i = 0; i < numTabs; i++)
    printf("\t");
}

FbxString FBXManager::GetAttributeTypeName(FbxNodeAttribute::EType type)
{
  switch (type) {
  case FbxNodeAttribute::eUnknown: return "unidentified";
  case FbxNodeAttribute::eNull: return "null";
  case FbxNodeAttribute::eMarker: return "marker";
  case FbxNodeAttribute::eSkeleton: return "skeleton";
  case FbxNodeAttribute::eMesh: return "mesh";
  case FbxNodeAttribute::eNurbs: return "nurbs";
  case FbxNodeAttribute::ePatch: return "patch";
  case FbxNodeAttribute::eCamera: return "camera";
  case FbxNodeAttribute::eCameraStereo: return "stereo";
  case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
  case FbxNodeAttribute::eLight: return "light";
  case FbxNodeAttribute::eOpticalReference: return "optical reference";
  case FbxNodeAttribute::eOpticalMarker: return "marker";
  case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
  case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
  case FbxNodeAttribute::eBoundary: return "boundary";
  case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
  case FbxNodeAttribute::eShape: return "shape";
  case FbxNodeAttribute::eLODGroup: return "lodgroup";
  case FbxNodeAttribute::eSubDiv: return "subdiv";
  default: return "unknown";
  }
}

void FBXManager::PrintAttribute(FbxNodeAttribute* attribute)
{
  if (!attribute) return;

  FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
  FbxString attrName = attribute->GetName();
  PrintTabs();
  // Note: to retrieve the character array of a FbxString, use its Buffer() method.
  printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void FBXManager::PrintNode(FbxNode* node)
{
  PrintTabs();
  const char* nodeName = node->GetName();
  FbxDouble3 translation = node->LclTranslation.Get();
  FbxDouble3 rotation = node->LclRotation.Get();
  FbxDouble3 scaling = node->LclScaling.Get();

  // Print the contents of the node.
  printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
    nodeName,
    translation[0], translation[1], translation[2],
    rotation[0], rotation[1], rotation[2],
    scaling[0], scaling[1], scaling[2]
  );
  numTabs++;

  // Print the node's attributes.
  for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    PrintAttribute(node->GetNodeAttributeByIndex(i));

  // Recursively print the children.
  for (int j = 0; j < node->GetChildCount(); j++)
    PrintNode(node->GetChild(j));

  numTabs--;
  PrintTabs();
  printf("</node>\n");
}



