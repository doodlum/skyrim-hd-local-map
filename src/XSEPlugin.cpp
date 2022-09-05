
std::uintptr_t MainProperties;

struct Hooks
{
	struct BSShaderRenderTargets_Create_MAIN
	{
		static void thunk(void* BSShaderRenderTargets, uint32_t TargetIndex, std::uintptr_t Properties)
		{
			MainProperties = Properties;
			func(BSShaderRenderTargets, TargetIndex, Properties);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct BSShaderRenderTargets_Create_LOCALMAP
	{
		static void thunk(void* BSShaderRenderTargets, uint32_t TargetIndex, [[maybe_unused]] std::uintptr_t Properties)
		{
			func(BSShaderRenderTargets, TargetIndex, MainProperties);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct BSShaderRenderTargets_Create_LOCALMAPSWAP
	{
		static void thunk(void* BSShaderRenderTargets, uint32_t TargetIndex, [[maybe_unused]] std::uintptr_t Properties)
		{
			func(BSShaderRenderTargets, TargetIndex, MainProperties);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	static void Install()
	{
		stl::write_thunk_call<BSShaderRenderTargets_Create_MAIN>(REL::RelocationID(100458, 107175).address() + REL::Relocate(0x3F0, 0x3F3));
		stl::write_thunk_call<BSShaderRenderTargets_Create_LOCALMAP>(REL::RelocationID(100458, 107175).address() + REL::Relocate(0x516, 0x57D));
		stl::write_thunk_call<BSShaderRenderTargets_Create_LOCALMAPSWAP>(REL::RelocationID(100458, 107175).address() + REL::Relocate(0x529, 0x593));
	}
};

void Init()
{
	Hooks::Install();
}

void InitializeLog()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		util::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= fmt::format("{}.log"sv, Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
	const auto level = spdlog::level::trace;
#else
	const auto level = spdlog::level::info;
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(level);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%l] %v"s);
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {};
#endif

	InitializeLog();

	logger::info("Loaded plugin");

	SKSE::Init(a_skse);

	Init();

	return true;
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) constinit auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName("PluginName");
	v.PluginVersion({ 1, 0, 0, 0 });
	v.UsesAddressLibrary(true);
	return v;
}();

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}
