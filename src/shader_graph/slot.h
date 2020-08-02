#pragma once

#include <cassert>
#include <cstddef>
#include <memory>

#include <boost/optional.hpp>

#include "shader_core/vector.h"

#include "curves.h"
#include "node_enums.h"
#include "ramp.h"

namespace csc {
	class FloatRect;
}

namespace csg {

	enum class SlotDirection {
		INPUT,
		OUTPUT,
	};

	enum class SlotType {
		BOOL,
		CLOSURE,
		COLOR,
		FLOAT,
		INT,
		VECTOR,
		ENUM,
		CURVE_RGB,
		CURVE_VECTOR,
		COLOR_RAMP,
	};

	class BoolSlotValue {
	public:
		BoolSlotValue(bool initial) : value{ initial } {}

		bool get() const { return value; }
		void set(bool new_value) { value = new_value; }

		bool operator==(const BoolSlotValue& other) const { return value == other.value; }
		bool operator!=(const BoolSlotValue& other) const { return operator==(other) == false; }

	private:
		bool value;
	};

	class ColorSlotValue {
	public:
		ColorSlotValue(csc::Float3 initial) : value{ initial } {}

		csc::Float3 get() const { return value; }
		void set(csc::Float3 new_value) { value = new_value; }

		bool operator==(const ColorSlotValue& other) const;
		bool operator!=(const ColorSlotValue& other) const { return operator==(other) == false; }

	private:
		csc::Float3 value;
	};

	class EnumSlotValue {
	public:
		// Use an explicit constructor for each available enum
		// Color
		EnumSlotValue(MixRGBType initial) :                     EnumSlotValue(NodeMetaEnum::MIX_RGB_TYPE, initial) {}
		// Converter
		EnumSlotValue(MapRangeType initial) :                   EnumSlotValue(NodeMetaEnum::MAP_RANGE_TYPE, initial) {}
		EnumSlotValue(MathType initial) :                       EnumSlotValue(NodeMetaEnum::MATH_TYPE, initial) {}
		EnumSlotValue(VectorMathType initial) :                 EnumSlotValue(NodeMetaEnum::VECTOR_MATH_TYPE, initial) {}
		// Input
		EnumSlotValue(TangentDirection initial) :               EnumSlotValue(NodeMetaEnum::TANGENT_DIRECTION, initial) {}
		EnumSlotValue(TangentAxis initial) :                    EnumSlotValue(NodeMetaEnum::TANGENT_AXIS, initial) {}
		// Shader
		EnumSlotValue(AnisotropicDistribution initial) :        EnumSlotValue(NodeMetaEnum::ANISOTROPIC_DISTRIBUTION, initial) {}
		EnumSlotValue(GlassDistribution initial) :              EnumSlotValue(NodeMetaEnum::GLASS_DISTRIBUTION, initial) {}
		EnumSlotValue(GlossyDistribution initial) :             EnumSlotValue(NodeMetaEnum::GLOSSY_DISTRIBUTION, initial) {}
		EnumSlotValue(HairComponent initial) :                  EnumSlotValue(NodeMetaEnum::HAIR_COMPONENT, initial) {}
		EnumSlotValue(PrincipledBSDFDistribution initial) :     EnumSlotValue(NodeMetaEnum::PRINCIPLED_BSDF_DISTRIBUTION, initial) {}
		EnumSlotValue(PrincipledBSDFSubsurfaceMethod initial) : EnumSlotValue(NodeMetaEnum::PRINCIPLED_BSDF_SSS, initial) {}
		EnumSlotValue(PrincipledHairColoring initial) :         EnumSlotValue(NodeMetaEnum::PRINCIPLED_HAIR_COLORING, initial) {}
		EnumSlotValue(RefractionDistribution initial) :         EnumSlotValue(NodeMetaEnum::REFRACTION_DISTRIBUTION, initial) {}
		EnumSlotValue(SubsurfaceScatterFalloff initial) :       EnumSlotValue(NodeMetaEnum::SSS_FALLOFF, initial) {}
		EnumSlotValue(ToonComponent initial) :                  EnumSlotValue(NodeMetaEnum::TOON_COMPONENT, initial) {}
		// Texture
		EnumSlotValue(MaxTexmapPrecision initial) :             EnumSlotValue(NodeMetaEnum::MAX_TEXMAP_PRECISION, initial) {}
		// Vector
		EnumSlotValue(DisplacementSpace initial) :              EnumSlotValue(NodeMetaEnum::DISPLACEMENT_SPACE, initial) {}
		EnumSlotValue(VectorMappingType initial) :              EnumSlotValue(NodeMetaEnum::VECTOR_MAPPING_TYPE, initial) {}
		EnumSlotValue(NormalMapSpace initial) :                 EnumSlotValue(NodeMetaEnum::NORMAL_MAP_SPACE, initial) {}
		EnumSlotValue(VectorDisplacementSpace initial) :        EnumSlotValue(NodeMetaEnum::VECTOR_DISPLACEMENT_SPACE, initial) {}
		EnumSlotValue(VectorTransformType initial) :            EnumSlotValue(NodeMetaEnum::VECTOR_TRANSFORM_TYPE, initial) {}
		EnumSlotValue(VectorTransformSpace initial) :           EnumSlotValue(NodeMetaEnum::VECTOR_TRANSFORM_SPACE, initial) {}

		NodeMetaEnum get_meta() const { return meta_enum; }
		size_t get() const { return value; }

		void set(size_t new_value);

		size_t max() const { return max_value; }

		const char* internal_name() const;

		bool operator==(const EnumSlotValue& other) const;
		bool operator!=(const EnumSlotValue& other) const { return operator==(other) == false; }

	private:
		EnumSlotValue(NodeMetaEnum meta_enum, size_t value, size_t max_value) :
			meta_enum{ meta_enum }, value{ value }, max_value{ max_value }
		{}
		template <typename T> EnumSlotValue(NodeMetaEnum meta_enum, T value) :
			EnumSlotValue(meta_enum, static_cast<size_t>(value), static_cast<size_t>(T::COUNT) - 1)
		{}

		NodeMetaEnum meta_enum;
		size_t value;
		size_t max_value;
	};

	class FloatSlotValue {
	public:
		FloatSlotValue(float initial, float min, float max, size_t precision = 3) :
			value{ initial }, min{ min }, max{ max }, _precision{ precision }
		{}

		float get() const { return value; }
		void set(float new_value);

		size_t precision() const { return _precision; }

		bool operator==(const FloatSlotValue& other) const;
		bool operator!=(const FloatSlotValue& other) const { return operator==(other) == false; }

	private:
		float value;
		float min;
		float max;
		size_t _precision;
	};

	class IntSlotValue {
	public:
		IntSlotValue(int initial, int min, int max) :
			value{ initial }, min{ min }, max{ max }
		{}

		int get() const { return value; }
		void set(int new_value);

		bool operator==(const IntSlotValue& other) const;
		bool operator!=(const IntSlotValue& other) const { return operator==(other) == false; }

	private:
		int value;
		int min;
		int max;
	};

	class VectorSlotValue {
	public:
		VectorSlotValue(csc::Float3 initial, csc::Float3 min, csc::Float3 max, size_t precision = 3) :
			value{ initial }, min{ min }, max{ max }, _precision{ precision }
			{}

		csc::Float3 get() const { return value; }
		void set(csc::Float3 new_value);

		size_t precision() const { return _precision; }

		bool operator==(const VectorSlotValue& other) const;
		bool operator!=(const VectorSlotValue& other) const { return operator==(other) == false; }

	private:
		csc::Float3 value;
		csc::Float3 min;
		csc::Float3 max;
		size_t _precision;
	};

	class RGBCurveSlotValue {
	public:
		RGBCurveSlotValue();

		Curve get_all() const { return curve_all; }
		Curve get_r() const { return curve_r; }
		Curve get_g() const { return curve_g; }
		Curve get_b() const { return curve_b; }

		void set(const RGBCurveSlotValue& value) { *this = RGBCurveSlotValue{ value }; }
		bool set_all (const Curve& value);
		bool set_r(const Curve& value);
		bool set_g(const Curve& value);
		bool set_b(const Curve& value);

		bool operator==(const RGBCurveSlotValue& other) const;
		bool operator!=(const RGBCurveSlotValue& other) const { return operator==(other) == false; }

	private:
		Curve curve_all;
		Curve curve_r;
		Curve curve_g;
		Curve curve_b;
	};

	class VectorCurveSlotValue {
	public:
		VectorCurveSlotValue(csc::Float2 min, csc::Float2 max);

		Curve get_x() const { return curve_x; }
		Curve get_y() const { return curve_y; }
		Curve get_z() const { return curve_z; }

		csc::Float2 get_min() const { return min; }
		csc::Float2 get_max() const { return max; }

		void set(const VectorCurveSlotValue& value) { *this = VectorCurveSlotValue{ value }; }
		bool set_x(const Curve& value);
		bool set_y(const Curve& value);
		bool set_z(const Curve& value);
		bool set_bounds(csc::FloatRect bounds_rect);

		bool operator==(const VectorCurveSlotValue& other) const;
		bool operator!=(const VectorCurveSlotValue& other) const { return operator==(other) == false; }

	private:
		Curve curve_x;
		Curve curve_y;
		Curve curve_z;
		csc::Float2 min;
		csc::Float2 max;
	};

	class ColorRampSlotValue {
	public:
		ColorRampSlotValue() {}
		ColorRampSlotValue(ColorRamp ramp) : ramp{ ramp } {}

		ColorRamp get() const { return ramp; }
		void set(ColorRamp new_ramp) { ramp = new_ramp; }
		void set(ColorRampSlotValue new_ramp) { *this = new_ramp; }

		bool operator==(const ColorRampSlotValue& other) const;
		bool operator!=(const ColorRampSlotValue& other) const { return operator==(other) == false; }

	private:
		ColorRamp ramp;
	};

	class SlotValue {
	public:
		SlotValue(BoolSlotValue bool_value) :     _type{ SlotType::BOOL },   value_union{ bool_value } {}
		SlotValue(ColorSlotValue color_value) :   _type{ SlotType::COLOR },  value_union{ color_value } {}
		SlotValue(EnumSlotValue enum_value) :     _type{ SlotType::ENUM },   value_union{ enum_value } {}
		SlotValue(FloatSlotValue float_value) :   _type{ SlotType::FLOAT },  value_union{ float_value } {}
		SlotValue(IntSlotValue int_value) :       _type{ SlotType::INT }, value_union{ int_value } {}
		SlotValue(VectorSlotValue vector_value) : _type{ SlotType::VECTOR }, value_union{ vector_value } {}

		SlotValue(const RGBCurveSlotValue& curve_value) : _type{ SlotType::CURVE_RGB }, curve_rgb_value{ std::make_unique<RGBCurveSlotValue>(curve_value) } {}
		SlotValue(const VectorCurveSlotValue& curve_value) : _type{ SlotType::CURVE_VECTOR }, curve_vector_value{ std::make_unique<VectorCurveSlotValue>(curve_value) } {}
		SlotValue(const ColorRampSlotValue& ramp_value) : _type{ SlotType::COLOR_RAMP }, color_ramp_value{ std::make_unique<ColorRampSlotValue>(ramp_value) } {}

		// Copy constructor and copy assignment operator, constructor defers to assignment
		SlotValue(const SlotValue& other) : value_union{ FloatSlotValue{ 0.0f, 0.0f, 0.0f } } { this->operator=(other); }
		SlotValue& operator=(const SlotValue& other);

		SlotType type() const { return _type; }

		// Base template function to get this object's value
		// Needs to be specialized for each type
		template <typename T> boost::optional<T> as() const { assert(false); }

		bool operator==(const SlotValue& other) const;
		bool operator!=(const SlotValue& other) const { return operator==(other) == false; }

		static size_t sizeof_union() { return sizeof(SlotValueUnion); }

	private:
		union SlotValueUnion {
		public:
			SlotValueUnion() {}
			SlotValueUnion(BoolSlotValue bool_value) : bool_value{ bool_value } {}
			SlotValueUnion(ColorSlotValue color_value) : color_value{ color_value } {}
			SlotValueUnion(EnumSlotValue enum_value) :   enum_value{ enum_value } {}
			SlotValueUnion(FloatSlotValue float_value) : float_value{ float_value } {}
			SlotValueUnion(IntSlotValue int_value) : int_value{ int_value } {}
			SlotValueUnion(VectorSlotValue vector_value) : vector_value{ vector_value } {}

			BoolSlotValue bool_value;
			ColorSlotValue color_value;
			EnumSlotValue  enum_value;
			FloatSlotValue float_value;
			IntSlotValue int_value;
			VectorSlotValue vector_value;
		};
		SlotType _type;
		SlotValueUnion value_union;

		// Pointers to non-union types here
		// Be sure to update copy assignment operator when a new pointer is added
		// These types are not part of the union because they contain heap-allocated resources that must be freed
		std::unique_ptr<RGBCurveSlotValue> curve_rgb_value;
		std::unique_ptr<VectorCurveSlotValue> curve_vector_value;
		std::unique_ptr<ColorRampSlotValue> color_ramp_value;
	};

	template <> boost::optional<BoolSlotValue> SlotValue::as() const;
	template <> boost::optional<ColorSlotValue> SlotValue::as() const;
	template <> boost::optional<EnumSlotValue> SlotValue::as() const;
	template <> boost::optional<FloatSlotValue> SlotValue::as() const;
	template <> boost::optional<IntSlotValue> SlotValue::as() const;
	template <> boost::optional<VectorSlotValue> SlotValue::as() const;
	template <> boost::optional<RGBCurveSlotValue> SlotValue::as() const;
	template <> boost::optional<VectorCurveSlotValue> SlotValue::as() const;
	template <> boost::optional<ColorRampSlotValue> SlotValue::as() const;

	class Slot {
	public:
		// Creates a slot that does not have an editable value
		Slot(const char* disp_name, const char* name, SlotDirection dir, SlotType type, bool has_pin = true) :
			_disp_name{ disp_name }, _name{ name }, _dir{ dir }, _type{ type }, _has_pin{ has_pin }
		{}
		// These constructors take value of a specific type, which can be edited
		// When a value is specificed, the slot is automatically treated as input
		Slot(const char* disp_name, const char* name, BoolSlotValue bool_value, bool has_pin = false) :
			value{ bool_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::BOOL }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, ColorSlotValue color_value, bool has_pin = true) :
			value{ color_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::COLOR }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, EnumSlotValue enum_value, bool has_pin = false) :
			value{ enum_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::ENUM }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, FloatSlotValue float_value, bool has_pin = true) :
			value{ float_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::FLOAT }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, IntSlotValue int_value, bool has_pin = false) :
			value{ int_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::INT }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, VectorSlotValue vector_value, bool has_pin = true) :
			value{ vector_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::VECTOR }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, RGBCurveSlotValue rgb_value, bool has_pin = false) :
			value{ rgb_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::CURVE_RGB }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, VectorCurveSlotValue vector_value, bool has_pin = false) :
			value{ vector_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::CURVE_VECTOR }, _has_pin{ has_pin }
		{}
		Slot(const char* disp_name, const char* name, ColorRampSlotValue ramp_value, bool has_pin = false) :
			value{ ramp_value }, _disp_name{ disp_name }, _name{ name }, _dir{ SlotDirection::INPUT }, _type{ SlotType::COLOR_RAMP }, _has_pin{ has_pin }
		{}

		SlotDirection dir() const { return _dir; }
		SlotType type() const { return _type; }
		const char* disp_name() const { return _disp_name; }
		const char* name() const { return _name; }
		bool has_pin() const { return _has_pin; }

		bool operator==(const Slot& other) const;
		bool operator!=(const Slot& other) const { return operator==(other) == false; }

		boost::optional<SlotValue> value;

	private:
		const char* _disp_name;
		const char* _name;
		SlotDirection _dir;
		SlotType _type;
		bool _has_pin;
	};
}
