#include "I18N.hpp"
#include <filesystem>
#include <iostream>
#include <cstdlib>

namespace fs = std::filesystem;

void SetEnvironmentVariable(const std::string& name, const std::string& value) {
#ifdef _WIN32
    _putenv_s(name.c_str(), value.c_str());
#else
    setenv(name.c_str(), value.c_str(), 1);
#endif
}

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
