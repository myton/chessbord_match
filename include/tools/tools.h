#ifndef TOOL_H
#define TOOL_H
#include <iostream>
#include <string>
#include <vector>
namespace tools
{
  int load_image(const std::string& path, int img_count, std::vector<std::pair<std::string, std::string>>& vstereo_img);
}
#endif
