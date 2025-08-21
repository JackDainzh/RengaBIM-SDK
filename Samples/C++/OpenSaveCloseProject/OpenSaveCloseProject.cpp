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

  // Saving project as
  pProject->Save();

  wchar_t buffer[MAX_PATH];
  ::GetCurrentDirectory(MAX_PATH, buffer);

  const auto filePathToSave = std::wstring(buffer) + L"/project.rnp";
  
  // Saving project as
  pProject->SaveAs(filePathToSave.c_str(), Renga::ProjectType::ProjectType_Project, VARIANT_FALSE);

  // Closing project
  renga->CloseProject(VARIANT_TRUE);

  // Closing Renga
  renga->Quit();

  ::CoUninitialize();
}
