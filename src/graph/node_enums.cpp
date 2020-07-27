#include "node_enums.h"

#include <utility>

#include <boost/optional.hpp>

template <typename T> static boost::optional<T> as_enum(const size_t option)
{
	if (option >= static_cast<size_t>(T::COUNT)) {
		return boost::none;
	}
	return static_cast<T>(option);
}

template <typename T> static size_t get_count()
{
	return static_cast<size_t>(T::COUNT);
}

boost::optional<csg::NodeEnumInfo> csg::NodeEnumInfo::from(const NodeMetaEnum meta_enum)
{
	if (meta_enum >= NodeMetaEnum::COUNT) {
		return boost::none;
	}

	switch (meta_enum) {
	// Color
	case NodeMetaEnum::MIX_RGB_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<MixRGBType>() };
	// Shader
	case NodeMetaEnum::ANISOTROPIC_DISTRIBUTION:
		return NodeEnumInfo{ meta_enum, get_count<AnisotropicDistribution>() };
	case NodeMetaEnum::GLASS_DISTRIBUTION:
		return NodeEnumInfo{ meta_enum, get_count<GlassDistribution>() };
	case NodeMetaEnum::GLOSSY_DISTRIBUTION:
		return NodeEnumInfo{ meta_enum, get_count<GlossyDistribution>() };
	case NodeMetaEnum::HAIR_COMPONENT:
		return NodeEnumInfo{ meta_enum, get_count<HairComponent>() };
	case NodeMetaEnum::PRINCIPLED_BSDF_DISTRIBUTION:
		return NodeEnumInfo{ meta_enum, get_count<PrincipledBSDFDistribution>() };
	case NodeMetaEnum::PRINCIPLED_BSDF_SSS:
		return NodeEnumInfo{ meta_enum, get_count<PrincipledBSDFSubsurfaceMethod>() };
	case NodeMetaEnum::PRINCIPLED_HAIR_COLORING:
		return NodeEnumInfo{ meta_enum, get_count<PrincipledHairColoring>() };
	case NodeMetaEnum::REFRACTION_DISTRIBUTION:
		return NodeEnumInfo{ meta_enum, get_count<RefractionDistribution>() };
	case NodeMetaEnum::SSS_FALLOFF:
		return NodeEnumInfo{ meta_enum, get_count<SubsurfaceScatterFalloff>() };
	case NodeMetaEnum::TOON_COMPONENT:
		return NodeEnumInfo{ meta_enum, get_count<ToonComponent>() };
	// Vector
	case NodeMetaEnum::DISPLACEMENT_SPACE:
		return NodeEnumInfo{ meta_enum, get_count<DisplacementSpace>() };
	case NodeMetaEnum::VECTOR_MAPPING_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<VectorMappingType>() };
	case NodeMetaEnum::NORMAL_MAP_SPACE:
		return NodeEnumInfo{ meta_enum, get_count<NormalMapSpace>() };
	case NodeMetaEnum::VECTOR_DISPLACEMENT_SPACE:
		return NodeEnumInfo{ meta_enum, get_count<VectorDisplacementSpace>() };
	case NodeMetaEnum::VECTOR_TRANSFORM_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<VectorTransformType>() };
	case NodeMetaEnum::VECTOR_TRANSFORM_SPACE:
		return NodeEnumInfo{ meta_enum, get_count<VectorTransformSpace>() };
	default:
		return boost::none;
	}
}

boost::optional<csg::NodeEnumOptionInfo> csg::NodeEnumOptionInfo::from(NodeMetaEnum meta_enum, size_t option)
{
	const boost::optional<NodeEnumInfo> enum_info{ NodeEnumInfo::from(meta_enum) };
	if (enum_info.has_value() == false) {
		return boost::none;
	}
	if (option >= enum_info->count()) {
		return boost::none;
	}

	return NodeEnumOptionInfo{ meta_enum, option };
}

// Tiny macro to make it easier to copy paste all these definitions
#define CASE_PAIR(ENUM, S1, S2) case ENUM: return std::make_pair(S1, S2);

static std::pair<const char*, const char*> get_option_names(csg::NodeMetaEnum meta_enum, size_t option)
{
	using namespace csg;

	switch (meta_enum) {

	//////
	// Color
	//////
	case NodeMetaEnum::MIX_RGB_TYPE:
	{
		const auto opt_enum{ as_enum<MixRGBType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(MixRGBType::MIX,          "Mix",          "mix");
				CASE_PAIR(MixRGBType::DARKEN,       "Darken",       "darken");
				CASE_PAIR(MixRGBType::MULTIPLY,     "Multiply",     "multiply");
				CASE_PAIR(MixRGBType::BURN,         "Burn",         "burn");
				CASE_PAIR(MixRGBType::LIGHTEN,      "Lighten",      "lighten");
				CASE_PAIR(MixRGBType::SCREEN,       "Screen",       "screen");
				CASE_PAIR(MixRGBType::DODGE,        "Dodge",        "dodge");
				CASE_PAIR(MixRGBType::ADD,          "Add",          "add");
				CASE_PAIR(MixRGBType::OVERLAY,      "Overlay",      "overlay");
				CASE_PAIR(MixRGBType::SOFT_LIGHT,   "Soft Light",   "soft_light");
				CASE_PAIR(MixRGBType::LINEAR_LIGHT, "Linear Light", "linear_light");
				CASE_PAIR(MixRGBType::DIFFERENCE,   "Difference",   "difference");
				CASE_PAIR(MixRGBType::SUBTRACT,     "Subtract",     "subtract");
				CASE_PAIR(MixRGBType::DIVIDE,       "Divide",       "divide");
				CASE_PAIR(MixRGBType::HUE,          "Hue",          "hue");
				CASE_PAIR(MixRGBType::SATURATION,   "Saturation",   "saturation");
				CASE_PAIR(MixRGBType::COLOR,        "Color",        "color");
				CASE_PAIR(MixRGBType::VALUE,        "Value",        "value");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	//////
	// Shader
	//////
	case NodeMetaEnum::ANISOTROPIC_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<AnisotropicDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(AnisotropicDistribution::ASHIKHMIN_SHIRLEY, "Ashikhmin-Shirley", "ashikhmin_shirley");
				CASE_PAIR(AnisotropicDistribution::BECKMANN,          "Beckmann",          "beckmann");
				CASE_PAIR(AnisotropicDistribution::GGX,               "GGX",               "ggx");
				CASE_PAIR(AnisotropicDistribution::MULTISCATTER_GGX,  "Multiscatter GGX",  "multiscatter_ggx");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case NodeMetaEnum::GLASS_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<GlassDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(GlassDistribution::GGX,              "GGX",              "ggx");
				CASE_PAIR(GlassDistribution::MULTISCATTER_GGX, "Multiscatter GGX", "multiscatter_ggx");
				CASE_PAIR(GlassDistribution::BECKMANN,         "Beckmann",         "beckmann");
				CASE_PAIR(GlassDistribution::SHARP,            "Sharp",            "sharp");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case NodeMetaEnum::GLOSSY_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<GlossyDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(GlossyDistribution::ASHIKHMIN_SHIRLEY, "Ashikhmin-Shirley", "ashikhmin_shirley");
				CASE_PAIR(GlossyDistribution::BECKMANN,          "Beckmann",          "beckmann");
				CASE_PAIR(GlossyDistribution::GGX,               "GGX",               "ggx");
				CASE_PAIR(GlossyDistribution::MULTISCATTER_GGX,  "Multiscatter GGX",  "multiscatter_ggx");
				CASE_PAIR(GlossyDistribution::SHARP,             "Sharp",             "sharp");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case NodeMetaEnum::HAIR_COMPONENT:
	{
		const auto opt_enum{ as_enum<HairComponent>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(HairComponent::REFLECTION,   "Reflection",   "reflection");
				CASE_PAIR(HairComponent::TRANSMISSION, "Transmission", "transmission");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case NodeMetaEnum::PRINCIPLED_BSDF_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<PrincipledBSDFDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(PrincipledBSDFDistribution::GGX,              "GGX",              "ggx");
				CASE_PAIR(PrincipledBSDFDistribution::MULTISCATTER_GGX, "Multiscatter GGX", "multiscatter_ggx");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case NodeMetaEnum::PRINCIPLED_BSDF_SSS:
	{
		const auto opt_enum{ as_enum<PrincipledBSDFSubsurfaceMethod>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(PrincipledBSDFSubsurfaceMethod::BURLEY,      "Burley",      "burley");
				CASE_PAIR(PrincipledBSDFSubsurfaceMethod::RANDOM_WALK, "Random Walk", "random_walk");
			default:
				return std::make_pair("[Unknown Method]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Method]", "ERROR");
		}
	}
	case NodeMetaEnum::PRINCIPLED_HAIR_COLORING:
	{
		const auto opt_enum{ as_enum<PrincipledHairColoring>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(PrincipledHairColoring::ABSORPTION_COEFFICIENT, "Absorption coefficient", "absorption_coefficient");
				CASE_PAIR(PrincipledHairColoring::MELANIN_CONCENTRATION,  "Melanin concentration",  "melanin_concentration");
				CASE_PAIR(PrincipledHairColoring::DIRECT_COLORING,        "Direct coloring",        "direct_coloring");
			default:
				return std::make_pair("[Unknown Coloring]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Coloring]", "ERROR");
		}
	}
	case NodeMetaEnum::REFRACTION_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<RefractionDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(RefractionDistribution::BECKMANN, "Beckmann", "beckmann");
				CASE_PAIR(RefractionDistribution::GGX,      "GGX",      "ggx");
				CASE_PAIR(RefractionDistribution::SHARP,    "Sharp",    "sharp");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case NodeMetaEnum::SSS_FALLOFF:
	{
		const auto opt_enum{ as_enum<SubsurfaceScatterFalloff>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(SubsurfaceScatterFalloff::BURLEY,      "Burley",      "burley");
				CASE_PAIR(SubsurfaceScatterFalloff::CUBIC,       "Cubic",       "cubic");
				CASE_PAIR(SubsurfaceScatterFalloff::GAUSSIAN,    "Gaussian",    "gaussian");
				CASE_PAIR(SubsurfaceScatterFalloff::RANDOM_WALK, "Random Walk", "random_walk");
			default:
				return std::make_pair("[Unknown Falloff]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Falloff]", "ERROR");
		}
	}
	case NodeMetaEnum::TOON_COMPONENT:
	{
		const auto opt_enum{ as_enum<ToonComponent>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(ToonComponent::DIFFUSE, "Diffuse", "diffuse");
				CASE_PAIR(ToonComponent::GLOSSY,  "Glossy",  "glossy");
			default:
				return std::make_pair("[Unknown Component]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Component]", "ERROR");
		}
	}
	case NodeMetaEnum::DISPLACEMENT_SPACE:
	{
		const auto opt_enum{ as_enum<DisplacementSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(DisplacementSpace::OBJECT, "Object", "object");
				CASE_PAIR(DisplacementSpace::WORLD,  "World",  "world");
			default:
				return std::make_pair("[Unknown Space]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	case NodeMetaEnum::VECTOR_MAPPING_TYPE:
	{
		const auto opt_enum{ as_enum<VectorMappingType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(VectorMappingType::POINT,   "Point",   "point");
				CASE_PAIR(VectorMappingType::TEXTURE, "Texture", "texture");
				CASE_PAIR(VectorMappingType::VECTOR,  "Vector",  "vector");
				CASE_PAIR(VectorMappingType::NORMAL,  "Normal",  "normal");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::NORMAL_MAP_SPACE:
	{
		const auto opt_enum{ as_enum<NormalMapSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(NormalMapSpace::TANGENT, "Tangent", "tangent");
				CASE_PAIR(NormalMapSpace::OBJECT,  "Object",  "object");
				CASE_PAIR(NormalMapSpace::WORLD,   "World",   "world");
			default:
				return std::make_pair("[Unknown Space]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	case NodeMetaEnum::VECTOR_DISPLACEMENT_SPACE:
	{
		const auto opt_enum{ as_enum<VectorDisplacementSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(VectorDisplacementSpace::TANGENT, "Tangent", "tangent");
				CASE_PAIR(VectorDisplacementSpace::OBJECT,  "Object",  "object");
				CASE_PAIR(VectorDisplacementSpace::WORLD,   "World",   "world");
			default:
				return std::make_pair("[Unknown Space]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	case NodeMetaEnum::VECTOR_TRANSFORM_TYPE:
	{
		const auto opt_enum{ as_enum<VectorTransformType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(VectorTransformType::POINT, "Point", "point");
				CASE_PAIR(VectorTransformType::VECTOR, "Vector", "vector");
				CASE_PAIR(VectorTransformType::NORMAL, "Normal", "normal");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::VECTOR_TRANSFORM_SPACE:
	{
		const auto opt_enum{ as_enum<VectorTransformSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(VectorTransformSpace::CAMERA, "Camera", "camera");
				CASE_PAIR(VectorTransformSpace::OBJECT, "Object", "object");
				CASE_PAIR(VectorTransformSpace::WORLD,  "World",  "world");
			default:
				return std::make_pair("[Unknown Space]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	/*
	case NodeMetaEnum::COPY_PASTE:
	{
		const auto opt_enum{ as_enum<EnumName>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(Enum::OPTION1, "Option1", "option1");
				CASE_PAIR(Enum::OPTION2, "Option2", "option2");
			default:
				return std::make_pair("[Unknown]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad]", "ERROR");
		}
	}
	*/
	default:
		return std::make_pair("[Enum Error]", "ERROR");
	}
}

const char* csg::NodeEnumOptionInfo::display_name() const
{
	return get_option_names(_meta_enum, _option).first;
}

const char* csg::NodeEnumOptionInfo::internal_name() const
{
	return get_option_names(_meta_enum, _option).second;
}
