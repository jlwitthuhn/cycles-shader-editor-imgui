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
	// Converter
	case NodeMetaEnum::CLAMP_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<ClampType>() };
	case NodeMetaEnum::MAP_RANGE_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<MapRangeType>() };
	case NodeMetaEnum::MATH_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<MathType>() };
	case NodeMetaEnum::VECTOR_MATH_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<VectorMathType>() };
	// Input
	case NodeMetaEnum::TANGENT_DIRECTION:
		return NodeEnumInfo{ meta_enum, get_count<TangentDirection>() };
	case NodeMetaEnum::TANGENT_AXIS:
		return NodeEnumInfo{ meta_enum, get_count<TangentAxis>() };
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
	// Texture
	case NodeMetaEnum::MAX_TEXMAP_PRECISION:
		return NodeEnumInfo{ meta_enum, get_count<MaxTexmapPrecision>() };
	case NodeMetaEnum::GRADIENT_TEX_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<GradientTexType>() };
	case NodeMetaEnum::MUSGRAVE_TEX_DIMENSIONS:
		return NodeEnumInfo{ meta_enum, get_count<MusgraveTexDimensions>() };
	case NodeMetaEnum::MUSGRAVE_TEX_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<MusgraveTexType>() };
	case NodeMetaEnum::NOISE_TEX_DIMENSIONS:
		return NodeEnumInfo{ meta_enum, get_count<NoiseTexDimensions>() };
	case NodeMetaEnum::VORONOI_TEX_DIMENSIONS:
		return NodeEnumInfo{ meta_enum, get_count<VoronoiTexDimensions>() };
	case NodeMetaEnum::WAVE_TEX_TYPE:
		return NodeEnumInfo{ meta_enum, get_count<WaveTexType>() };
	case NodeMetaEnum::WAVE_TEX_DIRECTION:
		return NodeEnumInfo{ meta_enum, get_count<WaveTexDirection>() };
	case NodeMetaEnum::WAVE_TEX_PROFILE:
		return NodeEnumInfo{ meta_enum, get_count<WaveTexProfile>() };
	case NodeMetaEnum::WHITE_NOISE_TEX_DIMENSIONS:
		return NodeEnumInfo{ meta_enum, get_count<WhiteNoiseTexDimensions>() };
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

csg::NodeEnumInfo csg::NodeEnumInfo::from_assert(const NodeMetaEnum meta_enum)
{
	const auto result{ from(meta_enum) };
	assert(result.has_value());
	return *result;
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
	// Converter
	//////
	case NodeMetaEnum::CLAMP_TYPE:
	{
		const auto opt_enum{ as_enum<ClampType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(ClampType::MINMAX, "Min Max", "minmax");
				CASE_PAIR(ClampType::RANGE,  "Range",   "range");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::MAP_RANGE_TYPE:
	{
		const auto opt_enum{ as_enum<MapRangeType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(MapRangeType::LINEAR,        "Linear",        "linear");
				CASE_PAIR(MapRangeType::STEPPED,       "Stepped",       "stepped");
				CASE_PAIR(MapRangeType::SMOOTH_STEP,   "Smooth Step",   "smooth_step");
				CASE_PAIR(MapRangeType::SMOOTHER_STEP, "Smoother Step", "smoother_step");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::MATH_TYPE:
	{
		const auto opt_enum{ as_enum<MathType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(MathType::ADD,          "Add (2)",                "add");
				CASE_PAIR(MathType::SUBTRACT,     "Subtract (2)",           "subtract");
				CASE_PAIR(MathType::MULTIPLY,     "Multiply (2)",           "multiply");
				CASE_PAIR(MathType::DIVIDE,       "Divide (2)",             "divide");
				CASE_PAIR(MathType::MULTIPLY_ADD, "Multiply Add (2)",       "multiply_add");
				CASE_PAIR(MathType::SINE,         "Sine (1)",               "sine");
				CASE_PAIR(MathType::COSINE,       "Cosine (1)",             "cosine");
				CASE_PAIR(MathType::TANGENT,      "Tangent (1)",            "tangent");
				CASE_PAIR(MathType::ARCSINE,      "Arcsine (1)",            "arcsine");
				CASE_PAIR(MathType::ARCCOSINE,    "Arccosine (1)",          "arccosine");
				CASE_PAIR(MathType::ARCTANGENT,   "Arctangent (1)",         "arctangent");
				CASE_PAIR(MathType::ARCTAN2,      "Arctan2 (2)",            "arctan2");
				CASE_PAIR(MathType::SINH,         "Hyperbolic Sine (1)",    "sinh");
				CASE_PAIR(MathType::COSH,         "Hyperbolic Cosine (1)",  "cosh");
				CASE_PAIR(MathType::TANH,         "Hyperbolic Tangent (1)", "tanh");
				CASE_PAIR(MathType::POWER,        "Power (2)",              "power");
				CASE_PAIR(MathType::LOGARITHM,    "Logarithm (2)",          "logarithm");
				CASE_PAIR(MathType::MINIMUM,      "Maximum (2)",            "maximum");
				CASE_PAIR(MathType::MAXIMUM,      "Minimum (2)",            "minimum");
				CASE_PAIR(MathType::LESS_THAN,    "Less Than (2)",          "less_than");
				CASE_PAIR(MathType::GREATER_THAN, "Greater Than (2)",       "greater_than");
				CASE_PAIR(MathType::MODULO,       "Modulo (2)",             "modulo");
				CASE_PAIR(MathType::ABSOLUTE,     "Absolute (1)",           "absolute");
				CASE_PAIR(MathType::ROUND,        "Round (1)",              "round");
				CASE_PAIR(MathType::FLOOR,        "Floor (1)",              "floor");
				CASE_PAIR(MathType::CEIL,         "Ceil (1)",               "ceil");
				CASE_PAIR(MathType::FRACTION,     "Fraction (1)",           "fraction");
				CASE_PAIR(MathType::SQRT,         "Square Root (1)",        "sqrt");
				CASE_PAIR(MathType::INV_SQRT,     "Inverse Sqrt (1)",       "inv_sqrt");
				CASE_PAIR(MathType::SIGN,         "Sign (1)",               "sign");
				CASE_PAIR(MathType::EXPONENT,     "Exponent (1)",           "exponent");
				CASE_PAIR(MathType::RADIANS,      "To Radians (1)",         "radians");
				CASE_PAIR(MathType::DEGREES,      "To Degrees (1)",         "degrees");
				CASE_PAIR(MathType::TRUNC,        "Truncate (1)",           "truncate");
				CASE_PAIR(MathType::SNAP,         "Snap (2)",               "snap");
				CASE_PAIR(MathType::WRAP,         "Wrap (3)",               "wrap");
				CASE_PAIR(MathType::COMPARE,      "Compare (3)",            "compare");
				CASE_PAIR(MathType::PINGPONG,     "Ping Pong (3)",          "pingpong");
				CASE_PAIR(MathType::SMOOTH_MIN,   "Smooth Min (3)",         "smooth_min");
				CASE_PAIR(MathType::SMOOTH_MAX,   "Smooth Max (3)",         "smooth_max");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::VECTOR_MATH_TYPE:
	{
		const auto opt_enum{ as_enum<VectorMathType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(VectorMathType::ADD,           "Add (2)",           "add");
				CASE_PAIR(VectorMathType::SUBTRACT,      "Subtract (2)",      "subtract");
				CASE_PAIR(VectorMathType::MULTIPLY,      "Multiply (2)",      "multiply");
				CASE_PAIR(VectorMathType::DIVIDE,        "Divide (2)",        "divide");
				CASE_PAIR(VectorMathType::CROSS_PRODUCT, "Cross Product (2)", "cross_product");
				CASE_PAIR(VectorMathType::PROJECT,       "Project (2)",       "project");
				CASE_PAIR(VectorMathType::REFLECT,       "Reflect (2)",       "reflect");
				CASE_PAIR(VectorMathType::DOT_PRODUCT,   "Dot Product (2)",   "dot_product");
				CASE_PAIR(VectorMathType::DISTANCE,      "Distance (2)",      "distance");
				CASE_PAIR(VectorMathType::LENGTH,        "Length (1)",        "length");
				CASE_PAIR(VectorMathType::SCALE,         "Scale (1)",         "scale");
				CASE_PAIR(VectorMathType::NORMALIZE,     "Normalize (1)",     "normalize");
				CASE_PAIR(VectorMathType::SNAP,          "Snap (2)",          "snap");
				CASE_PAIR(VectorMathType::FLOOR,         "Floor (1)",         "floor");
				CASE_PAIR(VectorMathType::CEIL,          "Ceil (1)",          "ceil");
				CASE_PAIR(VectorMathType::MODULO,        "Modulo (2)",        "modulo");
				CASE_PAIR(VectorMathType::FRACTION,      "Fraction (1)",      "fraction");
				CASE_PAIR(VectorMathType::ABSOLUTE,      "Absolute (1)",      "absolute");
				CASE_PAIR(VectorMathType::MINIMUM,       "Minimum (2)",       "minimum");
				CASE_PAIR(VectorMathType::MAXIMUM,       "Maximum (2)",       "maximum");
				CASE_PAIR(VectorMathType::WRAP,          "Wrap (3)",          "wrap");
				CASE_PAIR(VectorMathType::SINE,          "Sine (1)",          "sine");
				CASE_PAIR(VectorMathType::COSINE,        "Cosine (1)",        "cosine");
				CASE_PAIR(VectorMathType::TANGENT,       "Tangent (1)",       "tangent");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	//////
	// Input
	//////
	case NodeMetaEnum::TANGENT_DIRECTION:
	{
		const auto opt_enum{ as_enum<TangentDirection>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(TangentDirection::RADIAL, "Radial", "radial");
				CASE_PAIR(TangentDirection::UV_MAP, "UV Map", "uv_map");
			default:
				return std::make_pair("[Unknown Direction]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Direction]", "ERROR");
		}
	}
	case NodeMetaEnum::TANGENT_AXIS:
	{
		const auto opt_enum{ as_enum<TangentAxis>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(TangentAxis::X, "X", "x");
				CASE_PAIR(TangentAxis::Y, "Y", "y");
				CASE_PAIR(TangentAxis::Z, "Z", "z");
			default:
				return std::make_pair("[Unknown Axis]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Axis]", "ERROR");
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
	//////
	// Texmap
	//////
	case NodeMetaEnum::MAX_TEXMAP_PRECISION:
	{
		const auto opt_enum{ as_enum<MaxTexmapPrecision>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(MaxTexmapPrecision::UCHAR, "8-bit/Channel Char",   "uchar");
				CASE_PAIR(MaxTexmapPrecision::FLOAT, "32-bit/Channel Float", "float");
			default:
				return std::make_pair("[Unknown Precision]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Precision]", "ERROR");
		}
	}
	case NodeMetaEnum::GRADIENT_TEX_TYPE:
	{
		const auto opt_enum{ as_enum<GradientTexType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(GradientTexType::LINEAR,           "Linear",           "linear");
				CASE_PAIR(GradientTexType::QUADRATIC,        "Quadratic",        "quadratic");
				CASE_PAIR(GradientTexType::EASING,           "Easing",           "easing");
				CASE_PAIR(GradientTexType::DIAGONAL,         "Diagonal",         "diagonal");
				CASE_PAIR(GradientTexType::RADIAL,           "Radial",           "radial");
				CASE_PAIR(GradientTexType::QUADRATIC_SPHERE, "Quadratic Shpere", "quadratic_sphere");
				CASE_PAIR(GradientTexType::SPHERICAL,        "Shperical",        "spherical");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::MUSGRAVE_TEX_DIMENSIONS:
	{
		const auto opt_enum{ as_enum<MusgraveTexDimensions>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(MusgraveTexDimensions::ONE,   "1D", "1D");
				CASE_PAIR(MusgraveTexDimensions::TWO,   "2D", "2D");
				CASE_PAIR(MusgraveTexDimensions::THREE, "3D", "3D");
				CASE_PAIR(MusgraveTexDimensions::FOUR,  "4D", "4D");
			default:
				return std::make_pair("[Unknown Dimensions]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Dimensions]", "ERROR");
		}
	}
	case NodeMetaEnum::MUSGRAVE_TEX_TYPE:
	{
		const auto opt_enum{ as_enum<MusgraveTexType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(MusgraveTexType::MULTIFRACTAL,        "Multifractal",        "multifractal");
				CASE_PAIR(MusgraveTexType::RIDGED_MULTIFRACTAL, "Ridged Multifractal", "ridged_multifractal");
				CASE_PAIR(MusgraveTexType::HYBRID_MULTIFRACTAL, "Hyvrid Multifractal", "hybrid_multifractal");
				CASE_PAIR(MusgraveTexType::FBM,                 "fBM",                 "fbm");
				CASE_PAIR(MusgraveTexType::HETERO_TERRAIN,      "Hetero Terrain",      "hetero_terrain");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::NOISE_TEX_DIMENSIONS:
	{
		const auto opt_enum{ as_enum<NoiseTexDimensions>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(NoiseTexDimensions::ONE, "1D", "1D");
				CASE_PAIR(NoiseTexDimensions::TWO, "2D", "2D");
				CASE_PAIR(NoiseTexDimensions::THREE, "3D", "3D");
				CASE_PAIR(NoiseTexDimensions::FOUR, "4D", "4D");
			default:
				return std::make_pair("[Unknown Dimensions]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Dimensions]", "ERROR");
		}
	}
	case NodeMetaEnum::VORONOI_TEX_DIMENSIONS:
	{
		const auto opt_enum{ as_enum<VoronoiTexDimensions>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(VoronoiTexDimensions::ONE, "1D", "1D");
				CASE_PAIR(VoronoiTexDimensions::TWO, "2D", "2D");
				CASE_PAIR(VoronoiTexDimensions::THREE, "3D", "3D");
				CASE_PAIR(VoronoiTexDimensions::FOUR, "4D", "4D");
			default:
				return std::make_pair("[Unknown Dimensions]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Dimensions]", "ERROR");
		}
	}
	case NodeMetaEnum::WAVE_TEX_TYPE:
	{
		const auto opt_enum{ as_enum<WaveTexType>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(WaveTexType::BANDS, "Bands", "bands");
				CASE_PAIR(WaveTexType::RINGS, "Rings", "rings");
			default:
				return std::make_pair("[Unknown Type]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Type]", "ERROR");
		}
	}
	case NodeMetaEnum::WAVE_TEX_DIRECTION:
	{
		const auto opt_enum{ as_enum<WaveTexDirection>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(WaveTexDirection::X, "X", "x");
				CASE_PAIR(WaveTexDirection::Y, "Y", "y");
				CASE_PAIR(WaveTexDirection::Z, "Z", "z");
				CASE_PAIR(WaveTexDirection::DIAGONAL, "Diagonal", "diagonal");
			default:
				return std::make_pair("[Unknown Direction]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Direction]", "ERROR");
		}
	}
	case NodeMetaEnum::WAVE_TEX_PROFILE:
	{
		const auto opt_enum{ as_enum<WaveTexProfile>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(WaveTexProfile::SINE,     "Sine",     "sine");
				CASE_PAIR(WaveTexProfile::SAW,      "Saw",      "saw");
				CASE_PAIR(WaveTexProfile::TRIANGLE, "Triangle", "triangle");
			default:
				return std::make_pair("[Unknown Profile]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Profile]", "ERROR");
		}
	}
	case NodeMetaEnum::WHITE_NOISE_TEX_DIMENSIONS:
	{
		const auto opt_enum{ as_enum<WhiteNoiseTexDimensions>(option) };
		if (opt_enum) {
			switch (*opt_enum) {
				CASE_PAIR(WhiteNoiseTexDimensions::ONE, "1D", "1D");
				CASE_PAIR(WhiteNoiseTexDimensions::TWO, "2D", "2D");
				CASE_PAIR(WhiteNoiseTexDimensions::THREE, "3D", "3D");
				CASE_PAIR(WhiteNoiseTexDimensions::FOUR, "4D", "4D");
			default:
				return std::make_pair("[Unknown Dimensions]", "ERROR");
			}
		}
		else {
			return std::make_pair("[Bad Dimensions]", "ERROR");
		}
	}
	//////
	// Vector
	//////
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
				CASE_PAIR(VectorTransformType::POINT,  "Point",  "point");
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
