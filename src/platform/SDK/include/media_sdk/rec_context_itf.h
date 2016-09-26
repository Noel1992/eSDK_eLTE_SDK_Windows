#ifndef MEDIA_SDK_REC_CONTEXT_ITF_HPP_
#define MEDIA_SDK_REC_CONTEXT_ITF_HPP_

#pragma warning(disable: 4251)

#include "media_export.h"
#include <string>

namespace ubp
{
namespace dc
{
namespace rec
{

const int CONST_1M = 1024 * 1024;

class MEDIA_IMPORT_EXPORT RecFileSessionInfo
{
  public:
    RecFileSessionInfo(std::string dir_path, std::string base_file_name, int max_file_size = 256 * CONST_1M);
    ~RecFileSessionInfo();

    std::string GetFilePath() const;

    int MaxFileSize() const
    {
      return max_file_size_;
    }

	RecFileSessionInfo(const RecFileSessionInfo& info);
  private:
    std::string dir_path_;
    std::string base_file_name_;
    int max_file_size_;
};

}
}
}

bool InitRecContext();
void ExitRecContext();

#endif