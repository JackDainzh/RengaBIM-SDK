#include <iostream>
#include <conio.h>
#include <string>

#import <RengaCOMAPI.tlb>

#include <Renga/CreateApplication.hpp>


int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "usage: " << argv[0] << " ProjectFullFilePath\n";
    return 0;
  }

  ::CoInitialize(nullptr);

  // Creating Renga
  auto renga = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);
  renga->PutVisible(VARIANT_TRUE);

  int result = renga->OpenProject(bstr_t(argv[1]));
  if (result)
  {
    std::cout << "Open file '" << argv[0] << "' error\n";
    renga->Quit();
    return 0;
  }

  auto pProject = renga->GetProject();

  wchar_t buffer[MAX_PATH];
  ::GetCurrentDirectory(MAX_PATH, buffer);

  auto pIfcExportSettings = renga->CreateIfcExportSettings();
  pIfcExportSettings->PutVersion(Renga::IfcVersion::IfcVersion_4);

  // Exporting to IFC
  const auto ifcFilePath = std::wstring(buffer) + L"/project.ifc";
  pProject->ExportToIfc2(ifcFilePath.c_str(), true, pIfcExportSettings);

  // Exporting drawings
  auto pDrawingCollection = pProject->GetDrawings();
  const auto size = pDrawingCollection->GetCount();
  for (int i = 0; i < size; ++i)
  {
    const auto dxfFilePath = std::wstring(buffer) + L"/drawing_ " + std::to_wstring(i) + L".dxf";
    const auto dwgFilePath = std::wstring(buffer) + L"/drawing_ " + std::to_wstring(i) + L".dwg";

    auto pDrawing = pDrawingCollection->Get(i);
    pDrawing->ExportToDxf(dxfFilePath.c_str(), Renga::AutocadVersion::AutocadVersion_v2000, true);
    pDrawing->ExportToDwg(dwgFilePath.c_str(), Renga::AutocadVersion::AutocadVersion_v2000, true);
  }

  // Closing Renga
  renga->Quit();

  ::CoUninitialize();
}
