/*
main
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
#include "dry/coreTests.h"
#include "Loop.h"


int main()
{
  AA::Loop testLoopA;

  return runAllCoreTests(testLoopA);

}

//////// FBX DEMO

//#include <fbxsdk.h>
//
///* Tab character ("\t") counter */
//int numTabs = 0;
//
///**
// * Print the required number of tabs.
// */
//void PrintTabs() {
//  for (int i = 0; i < numTabs; i++)
//    printf("\t");
//}
//
///**
// * Return a string-based representation based on the attribute type.
// */
//FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
//  switch (type) {
//  case FbxNodeAttribute::eUnknown: return "unidentified";
//  case FbxNodeAttribute::eNull: return "null";
//  case FbxNodeAttribute::eMarker: return "marker";
//  case FbxNodeAttribute::eSkeleton: return "skeleton";
//  case FbxNodeAttribute::eMesh: return "mesh";
//  case FbxNodeAttribute::eNurbs: return "nurbs";
//  case FbxNodeAttribute::ePatch: return "patch";
//  case FbxNodeAttribute::eCamera: return "camera";
//  case FbxNodeAttribute::eCameraStereo: return "stereo";
//  case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
//  case FbxNodeAttribute::eLight: return "light";
//  case FbxNodeAttribute::eOpticalReference: return "optical reference";
//  case FbxNodeAttribute::eOpticalMarker: return "marker";
//  case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
//  case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
//  case FbxNodeAttribute::eBoundary: return "boundary";
//  case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
//  case FbxNodeAttribute::eShape: return "shape";
//  case FbxNodeAttribute::eLODGroup: return "lodgroup";
//  case FbxNodeAttribute::eSubDiv: return "subdiv";
//  default: return "unknown";
//  }
//}
//
///**
// * Print an attribute.
// */
//void PrintAttribute(FbxNodeAttribute* pAttribute) {
//  if (!pAttribute) return;
//
//  FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
//  FbxString attrName = pAttribute->GetName();
//  PrintTabs();
//  // Note: to retrieve the character array of a FbxString, use its Buffer() method.
//  printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
//}
//
///**
// * Print a node, its attributes, and all its children recursively.
// */
//void PrintNode(FbxNode* pNode) {
//  PrintTabs();
//  const char* nodeName = pNode->GetName();
//  FbxDouble3 translation = pNode->LclTranslation.Get();
//  FbxDouble3 rotation = pNode->LclRotation.Get();
//  FbxDouble3 scaling = pNode->LclScaling.Get();
//
//  // Print the contents of the node.
//  printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
//    nodeName,
//    translation[0], translation[1], translation[2],
//    rotation[0], rotation[1], rotation[2],
//    scaling[0], scaling[1], scaling[2]
//  );
//  numTabs++;
//
//  // Print the node's attributes.
//  for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
//    PrintAttribute(pNode->GetNodeAttributeByIndex(i));
//
//  // Recursively print the children.
//  for (int j = 0; j < pNode->GetChildCount(); j++)
//    PrintNode(pNode->GetChild(j));
//
//  numTabs--;
//  PrintTabs();
//  printf("</node>\n");
//}
//
///**
// * Main function - loads the hard-coded fbx file,
// * and prints its contents in an xml format to stdout.
// */
//int main(int argc, char** argv) {
//
//  // Change the following filename to a suitable filename value.
//  //const char* lFilename = "file.fbx";
//  //const char* lFilename = "../../assets/erikaarcher/erika_archer_bow_arrow.fbx";
//  const char* lFilename = "E:/storage/source/repos/AncientArcher/assets/erikaarcher/erika_archer_bow_arrow.fbx";
//
//  // Initialize the SDK manager. This object handles all our memory management.
//  FbxManager* lSdkManager = FbxManager::Create();
//
//  // Create the IO settings object.
//  FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
//  lSdkManager->SetIOSettings(ios);
//
//  // Create an importer using the SDK manager.
//  FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
//
//  // Use the first argument as the filename for the importer.
//  if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
//    printf("Call to FbxImporter::Initialize() failed.\n");
//    printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
//    exit(-1);
//  }
//
//  // Create a new scene so that it can be populated by the imported file.
//  FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
//
//  // Import the contents of the file into the scene.
//  lImporter->Import(lScene);
//
//  // The file is imported; so get rid of the importer.
//  lImporter->Destroy();
//
//  // Print the nodes of the scene and their attributes recursively.
//  // Note that we are not printing the root node because it should
//  // not contain any attributes.
//  FbxNode* lRootNode = lScene->GetRootNode();
//  if (lRootNode) {
//    for (int i = 0; i < lRootNode->GetChildCount(); i++)
//      PrintNode(lRootNode->GetChild(i));
//  }
//  // Destroy the SDK manager and all the other objects it was handling.
//  lSdkManager->Destroy();
//
//  char a;
//  std::cin >> a;
//
//  return 0;
//}