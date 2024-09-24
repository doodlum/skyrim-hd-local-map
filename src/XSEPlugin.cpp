
RE::BSGraphics::RenderTargetProperties* MainProperties;

struct Hooks
{
	struct BSShaderRenderTargets_Create_MAIN
	{
		static void thunk(void* BSShaderRenderTargets, uint32_t TargetIndex, RE::BSGraphics::RenderTargetProperties* Properties)
		{
			logger::info("Storing main render target information");
			MainProperties = Properties;
			func(BSShaderRenderTargets, TargetIndex, Properties);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct BSShaderRenderTargets_Create_LOCALMAP
	{
		static void thunk(void* BSShaderRenderTargets, uint32_t TargetIndex, RE::BSGraphics::RenderTargetProperties* Properties)
		{
			logger::info("Patching LOCAL_MAP");

			Properties->width = MainProperties->width;
			Properties->height = MainProperties->height;

			func(BSShaderRenderTargets, TargetIndex, Properties);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct BSShaderRenderTargets_Create_LOCALMAPSWAP
	{
		static void thunk(void* BSShaderRenderTargets, uint32_t TargetIndex, RE::BSGraphics::RenderTargetProperties* Properties)
		{
			logger::info("Patching LOCAL_MAP_SWAP");

			Properties->width = MainProperties->width;
			Properties->height = MainProperties->height;

			func(BSShaderRenderTargets, TargetIndex, Properties);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	static void Install()
	{
		logger::info("Installing Hooks");
		stl::write_thunk_call<BSShaderRenderTargets_Create_MAIN>(REL::RelocationID(100458, 107175).address() + REL::Relocate(0x3F0, 0x3F3));
		stl::write_thunk_call<BSShaderRenderTargets_Create_LOCALMAP>(REL::RelocationID(100458, 107175).address() + REL::Relocate(0x57E, 0x57D));
		stl::write_thunk_call<BSShaderRenderTargets_Create_LOCALMAPSWAP>(REL::RelocationID(100458, 107175).address() + REL::Relocate(0x594, 0x593));
	}
};

void Load()
{
	Hooks::Install();
}
