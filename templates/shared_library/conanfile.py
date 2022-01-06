import re, os
import os
import platform
from conans import ConanFile, CMake, tools
from conans.tools import load

class Pkg(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires=[  
        "spdlog/1.9.2",
        ]
        
    generators = "cmake"

    def imports(self):
        self.copy("*.dll", dst="", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="", src="bin") # From lib to bin
        self.copy("*.so*", dst="", src="bin") # From lib to bin
        self.copy("*.ini*", dst="", src="bin") # From lib to bin
        
    def set_name(self):
        content = load(os.path.join(self.recipe_folder, "project.info"))
        self.name = re.search(r"project_name:[ \t]*([^ \t\r\n]*)", content).group(1)

    def set_version(self):
        content = load(os.path.join(self.recipe_folder, "project.info"))
        self.version = re.search(r"project_version:[ \t]*([0-9.]*)", content).group(1)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()


