#pragma once

#include <cstddef>

#include <boost/optional.hpp>

namespace csg {
	enum class NodeMetaEnum {
		// Color
		MIX_RGB_TYPE,
		// Converter
		MAP_RANGE_TYPE,
		MATH_TYPE,
		VECTOR_MATH_TYPE,
		// Input
		TANGENT_DIRECTION,
		TANGENT_AXIS,
		// Shader
		ANISOTROPIC_DISTRIBUTION,
		GLASS_DISTRIBUTION,
		GLOSSY_DISTRIBUTION,
		HAIR_COMPONENT,
		PRINCIPLED_BSDF_DISTRIBUTION,
		PRINCIPLED_BSDF_SSS,
		PRINCIPLED_HAIR_COLORING,
		REFRACTION_DISTRIBUTION,
		SSS_FALLOFF,
		TOON_COMPONENT,
		// Texture
		MAX_TEXMAP_PRECISION,
		// Vector
		DISPLACEMENT_SPACE,
		VECTOR_MAPPING_TYPE,
		NORMAL_MAP_SPACE,
		VECTOR_DISPLACEMENT_SPACE,
		VECTOR_TRANSFORM_TYPE,
		VECTOR_TRANSFORM_SPACE,
		// Bookkeeping
		COUNT
	};

	enum class MixRGBType {
		MIX,
		DARKEN,
		MULTIPLY,
		BURN,
		LIGHTEN,
		SCREEN,
		DODGE,
		ADD,
		OVERLAY,
		SOFT_LIGHT,
		LINEAR_LIGHT,
		DIFFERENCE,
		SUBTRACT,
		DIVIDE,
		HUE,
		SATURATION,
		COLOR,
		VALUE,
		COUNT
	};

	enum class MapRangeType {
		LINEAR,
		STEPPED,
		SMOOTH_STEP,
		SMOOTHER_STEP,
		COUNT
	};

	enum class MathType {
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		MULTIPLY_ADD,
		SINE,
		COSINE,
		TANGENT,
		ARCSINE,
		ARCCOSINE,
		ARCTANGENT,
		ARCTAN2,
		SINH,
		COSH,
		TANH,
		POWER,
		LOGARITHM,
		MINIMUM,
		MAXIMUM,
		LESS_THAN,
		GREATER_THAN,
		MODULO,
		ABSOLUTE,
		ROUND,
		FLOOR,
		CEIL,
		FRACTION,
		SQRT,
		INV_SQRT,
		SIGN,
		EXPONENT,
		RADIANS,
		DEGREES,
		TRUNC,
		SNAP,
		WRAP,
		COMPARE,
		PINGPONG,
		SMOOTH_MIN,
		SMOOTH_MAX,
		COUNT
	};

	enum class VectorMathType {
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		CROSS_PRODUCT,
		PROJECT,
		REFLECT,
		DOT_PRODUCT,
		DISTANCE,
		LENGTH,
		SCALE,
		NORMALIZE,
		SNAP,
		FLOOR,
		CEIL,
		MODULO,
		FRACTION,
		ABSOLUTE,
		MINIMUM,
		MAXIMUM,
		WRAP,
		SINE,
		COSINE,
		TANGENT,
		COUNT
	};

	enum class TangentDirection {
		RADIAL,
		UV_MAP,
		COUNT
	};

	enum class TangentAxis {
		X,
		Y,
		Z,
		COUNT
	};

	enum class AnisotropicDistribution {
		ASHIKHMIN_SHIRLEY,
		BECKMANN,
		GGX,
		MULTISCATTER_GGX,
		COUNT
	};

	enum class GlassDistribution {
		BECKMANN,
		GGX,
		MULTISCATTER_GGX,
		SHARP,
		COUNT
	};

	enum class GlossyDistribution {
		ASHIKHMIN_SHIRLEY,
		BECKMANN,
		GGX,
		MULTISCATTER_GGX,
		SHARP,
		COUNT
	};

	enum class HairComponent {
		TRANSMISSION,
		REFLECTION,
		COUNT
	};

	enum class PrincipledBSDFDistribution {
		GGX,
		MULTISCATTER_GGX,
		COUNT
	};

	enum class PrincipledBSDFSubsurfaceMethod {
		BURLEY,
		RANDOM_WALK,
		COUNT
	};

	enum class PrincipledHairColoring {
		ABSORPTION_COEFFICIENT,
		MELANIN_CONCENTRATION,
		DIRECT_COLORING,
		COUNT
	};

	enum class RefractionDistribution {
		BECKMANN,
		GGX,
		SHARP,
		COUNT
	};

	enum class SubsurfaceScatterFalloff {
		BURLEY,
		CUBIC,
		GAUSSIAN,
		RANDOM_WALK,
		COUNT
	};

	enum class ToonComponent {
		DIFFUSE,
		GLOSSY,
		COUNT
	};

	enum class MaxTexmapPrecision {
		UCHAR,
		FLOAT,
		COUNT
	};

	enum class DisplacementSpace {
		OBJECT,
		WORLD,
		COUNT
	};

	enum class VectorMappingType {
		POINT,
		TEXTURE,
		VECTOR,
		NORMAL,
		COUNT
	};

	enum class NormalMapSpace {
		TANGENT,
		OBJECT,
		WORLD,
		COUNT
	};

	enum class VectorDisplacementSpace {
		TANGENT,
		OBJECT,
		WORLD,
		COUNT
	};

	enum class VectorTransformType {
		POINT,
		VECTOR,
		NORMAL,
		COUNT
	};

	enum class VectorTransformSpace {
		CAMERA,
		OBJECT,
		WORLD,
		COUNT
	};

	class NodeEnumInfo {
	public:
		static boost::optional<NodeEnumInfo> from(NodeMetaEnum meta_enum);

		NodeMetaEnum meta_enum() const { return _meta_enum; }
		size_t count() const { return _count; }

	private:
		NodeEnumInfo(NodeMetaEnum meta_enum, size_t count) : _meta_enum{ meta_enum }, _count{ count } {}

		NodeMetaEnum _meta_enum;
		size_t _count;
	};

	class NodeEnumOptionInfo {
	public:
		static boost::optional<NodeEnumOptionInfo> from(NodeMetaEnum meta_enum, size_t option);

		const char* display_name() const;
		const char* internal_name() const;

	private:
		NodeEnumOptionInfo(NodeMetaEnum meta_enum, size_t option) : _meta_enum{ meta_enum }, _option{ option } {}

		NodeMetaEnum _meta_enum;
		size_t _option;
	};
}
