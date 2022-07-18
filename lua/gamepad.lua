workspace "gamepad"
	startproject "gamepad"
	location "../build/"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.platform}/"
	objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}-%{cfg.platform}/"
	buildlog "%{wks.location}/obj/%{cfg.platform}/%{cfg.buildcfg}-%{prj.name}.log"

	largeaddressaware "on"
	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "unicode"
	warnings "extra"

	flags {
		"noincrementallink",
		"no64bitchecks",
		"shadowedvariables",
		"undefinedidentifiers",
		"multiprocessorcompile",
	}

	platforms {
		"x86",
	}

	configurations {
		"Release",
		"Debug",
	}

	defines {
		"NOMINMAX",
		"WIN32_LEAN_AND_MEAN",
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	}

	--x86
	filter "platforms:x86"	
		architecture "x86"
	--end

	filter "Release"
		defines "NDEBUG"
		optimize "full"
		runtime "debug"
		symbols "off"

	filter "Debug"
		defines "DEBUG"
		optimize "debug"
		runtime "debug"
		symbols "on"

	project "gamepad"
		targetname "gamepad"
		language "c++"
		cppdialect "c++17"
		kind "sharedlib"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "../src/stdafx.cpp"
		forceincludes "stdafx.hpp"

		dependson {
			"MinHook",
			"Haggle",
		}

		syslibdirs {
			"../deps/SDL2-2.0.22/lib/x86"
		}

		links {
			"MinHook",
			"Haggle",
			"SDL2",
		}

		includedirs {
			"../src/",
			"../deps/minhook/include/",
			"../deps/haggle/src/haggle/",
			"../deps/SDL2-2.0.22/include/",
		}

		files {
			"../src/**",
		}

	group "Dependencies"

	project "MinHook"
		targetname "MinHook"

		language "c++"
		kind "staticlib"

		files {
			"../deps/minhook/src/**",
		}

		includedirs {
			"../deps/minhook/include/",
		}

	project "Haggle"
		targetname "haggle-sdk"
		language "c++"
		kind "sharedlib"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "../deps/haggle/src/haggle/stdafx.cpp"
		forceincludes "stdafx.hpp"

		dependson {
			"MinHook",
		}

		links {
			"MinHook",
		}

		includedirs {
			"../deps/haggle/src/haggle/",
			"../deps/minhook/include/",
		}

		files {
			"../deps/haggle/src/haggle/**",
		}
