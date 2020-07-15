#include "node_enums.h"

#include <utility>
#include <vector>

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

static std::pair<const char*, const char*> get_option_names(csg::NodeMetaEnum meta_enum, size_t option)
{
	switch (meta_enum) {

	//////
	// Shader
	//////
	case csg::NodeMetaEnum::ANISOTROPIC_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<csg::AnisotropicDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::AnisotropicDistribution::ASHIKHMIN_SHIRLEY:
				return std::make_pair("Ashikhmin-Shirley", "ashikhmin_shirley");
			case csg::AnisotropicDistribution::BECKMANN:
				return std::make_pair("Beckmann", "beckmann");
			case csg::AnisotropicDistribution::GGX:
				return std::make_pair("GGX", "ggx");
			case csg::AnisotropicDistribution::MULTISCATTER_GGX:
				return std::make_pair("Multiscatter GGX", "multiscatter_ggx");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::GLASS_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<csg::GlassDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::GlassDistribution::GGX:
				return std::make_pair("GGX", "ggx");
			case csg::GlassDistribution::MULTISCATTER_GGX:
				return std::make_pair("Multiscatter GGX", "multiscatter_ggx");
			case csg::GlassDistribution::BECKMANN:
				return std::make_pair("Beckmann", "beckmann");
			case csg::GlassDistribution::SHARP:
				return std::make_pair("Sharp", "sharp");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::GLOSSY_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<csg::GlossyDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::GlossyDistribution::ASHIKHMIN_SHIRLEY:
				return std::make_pair("Ashikhmin-Shirley", "ashikhmin_shirley");
			case csg::GlossyDistribution::BECKMANN:
				return std::make_pair("Beckmann", "beckmann");
			case csg::GlossyDistribution::GGX:
				return std::make_pair("GGX", "ggx");
			case csg::GlossyDistribution::MULTISCATTER_GGX:
				return std::make_pair("Multiscatter GGX", "multiscatter_ggx");
			case csg::GlossyDistribution::SHARP:
				return std::make_pair("Sharp", "sharp");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::HAIR_COMPONENT:
	{
		const auto opt_enum{ as_enum<csg::HairComponent>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::HairComponent::REFLECTION:
				return std::make_pair("Reflection", "reflection");
			case csg::HairComponent::TRANSMISSION:
				return std::make_pair("Transmission", "transmission");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::PRINCIPLED_BSDF_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<csg::PrincipledBSDFDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::PrincipledBSDFDistribution::GGX:
				return std::make_pair("GGX", "ggx");
			case csg::PrincipledBSDFDistribution::MULTISCATTER_GGX:
				return std::make_pair("Multiscatter GGX", "multiscatter_ggx");
			default:
				return std::make_pair("[Unknown Distribution]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::PRINCIPLED_BSDF_SSS:
	{
		const auto opt_enum{ as_enum<csg::PrincipledBSDFSubsurfaceMethod>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::PrincipledBSDFSubsurfaceMethod::BURLEY:
				return std::make_pair("Burley", "burley");
			case csg::PrincipledBSDFSubsurfaceMethod::RANDOM_WALK:
				return std::make_pair("Random Walk", "random_walk");
			default:
				return std::make_pair("[Unknown Method]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Method]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::PRINCIPLED_HAIR_COLORING:
	{
		const auto opt_enum{ as_enum<csg::PrincipledHairColoring>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::PrincipledHairColoring::ABSORPTION_COEFFICIENT:
				return std::make_pair("Absorption coefficient", "absorption_coefficient");
			case csg::PrincipledHairColoring::MELANIN_CONCENTRATION:
				return std::make_pair("Melanin concentration", "melanin_concentration");
			case csg::PrincipledHairColoring::DIRECT_COLORING:
				return std::make_pair("Direct coloring", "direct_coloring");
			default:
				return std::make_pair("[Unknown Coloring]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Coloring]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::REFRACTION_DISTRIBUTION:
	{
		const auto opt_enum{ as_enum<csg::RefractionDistribution>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::RefractionDistribution::BECKMANN:
				return std::make_pair("Beckmann", "beckmann");
			case csg::RefractionDistribution::GGX:
				return std::make_pair("GGX", "ggx");
			case csg::RefractionDistribution::SHARP:
				return std::make_pair("Sharp", "sharp");
			default:
				return std::make_pair("[Unknown]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Distribution]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::SSS_FALLOFF:
	{
		const auto opt_enum{ as_enum<csg::SubsurfaceScatterFalloff>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::SubsurfaceScatterFalloff::BURLEY:
				return std::make_pair("Burley", "burley");
			case csg::SubsurfaceScatterFalloff::CUBIC:
				return std::make_pair("Cubic", "cubic");
			case csg::SubsurfaceScatterFalloff::GAUSSIAN:
				return std::make_pair("Gaussian", "gaussian");
			case csg::SubsurfaceScatterFalloff::RANDOM_WALK:
				return std::make_pair("Random Walk", "random_walk");
			default:
				return std::make_pair("[Unknown]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Falloff]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::TOON_COMPONENT:
	{
		const auto opt_enum{ as_enum<csg::ToonComponent>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::ToonComponent::DIFFUSE:
				return std::make_pair("Diffuse", "diffuse");
			case csg::ToonComponent::GLOSSY:
				return std::make_pair("Glossy", "glossy");
			default:
				return std::make_pair("[Unknown]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Component]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::DISPLACEMENT_SPACE:
	{
		const auto opt_enum{ as_enum<csg::DisplacementSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::DisplacementSpace::OBJECT:
				return std::make_pair("Object Space", "object");
			case csg::DisplacementSpace::WORLD:
				return std::make_pair("World Space", "world");
			default:
				return std::make_pair("[Unknown]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::NORMAL_MAP_SPACE:
	{
		const auto opt_enum{ as_enum<csg::NormalMapSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::NormalMapSpace::TANGENT:
				return std::make_pair("Tangent", "tangent");
			case csg::NormalMapSpace::OBJECT:
				return std::make_pair("Object", "object");
			case csg::NormalMapSpace::WORLD:
				return std::make_pair("World", "world");
			default:
				return std::make_pair("[Unknown]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::VECTOR_DISPLACEMENT_SPACE:
	{
		const auto opt_enum{ as_enum<csg::VectorDisplacementSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::VectorDisplacementSpace::TANGENT:
				return std::make_pair("Tangent", "tangent");
			case csg::VectorDisplacementSpace::OBJECT:
				return std::make_pair("Object", "object");
			case csg::VectorDisplacementSpace::WORLD:
				return std::make_pair("World", "world");
			default:
				return std::make_pair("[Unknown Space]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::VECTOR_TRANSFORM_TYPE:
	{
		const auto opt_enum{ as_enum<csg::VectorTransformType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::VectorTransformType::POINT:
				return std::make_pair("Point", "point");
			case csg::VectorTransformType::VECTOR:
				return std::make_pair("Vector", "vector");
			case csg::VectorTransformType::NORMAL:
				return std::make_pair("Normal", "normal");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case csg::NodeMetaEnum::VECTOR_TRANSFORM_SPACE:
	{
		const auto opt_enum{ as_enum<csg::VectorTransformSpace>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::VectorTransformSpace::CAMERA:
				return std::make_pair("Camera", "camera");
			case csg::VectorTransformSpace::OBJECT:
				return std::make_pair("Object", "object");
			case csg::VectorTransformSpace::WORLD:
				return std::make_pair("World", "world");
			default:
				return std::make_pair("[Unknown Space]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Space]", "ERROR");
		}
	}
	/*
	case csg::NodeMetaEnum::COPY_PASTE:
	{
		const auto opt_enum{ as_enum<csg::EnumName>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
			case csg::EnumName::OPTION:
				return std::make_pair("Option", "option");
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
