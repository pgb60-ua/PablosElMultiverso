#include "I18N.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string GetLocalePath() {
    if (fs::exists("locale") && fs::is_directory("locale")) {
        return "./locale";
    }

    if (fs::exists("../locale") && fs::is_directory("../locale")) {
        return "../locale";
    }

    if (fs::exists("/usr/share/pablos-el-multiverso/locale")) {
        return "/usr/share/pablos-el-multiverso/locale";
    }
    
    if (fs::exists("/usr/share/locale")) {
        return "/usr/share/locale";
    }

    return "./locale";
}
