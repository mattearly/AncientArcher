#pragma once
#include <fbxsdk.h>
// for debug
int numTabs = 0;

void PrintTabs()
{
  for (int i = 0; i < numTabs; i++)
    printf("  ");
}

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
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

void PrintAttribute(FbxNodeAttribute* attribute)
{
  if (!attribute) return;

  FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
  FbxString attrName = attribute->GetName();
  PrintTabs();
  // Note: to retrieve the character array of a FbxString, use its Buffer() method.
  printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void PrintNode(FbxNode* node)
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



