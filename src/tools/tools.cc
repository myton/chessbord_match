#include <tools/tools.h>
#include <iomanip>
#include <algorithm>
#include <sstream>
namespace tools
{
  int load_image(const std::string& path, int img_count, std::vector<std::pair<std::string, std::string>>& vstereo_img)
  {
    const std::string& preFixPath = path;
    for (int i = 1; i <= img_count; ++i)
    {
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(2) << i;
      vstereo_img.emplace_back(preFixPath+"/left" + ss.str() + ".jpg", preFixPath+"/right" + ss.str() + ".jpg");
    }

    return vstereo_img.size();
  }
}
