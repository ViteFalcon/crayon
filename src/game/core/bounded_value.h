#pragma once

namespace crayon {
template <class Type>
class BoundedValue {
 public:
  BoundedValue(const Type min, const Type max) : _min(min), _max(max), _value(min) {}
  BoundedValue(const Type min, const Type max, const Type value) : _min(min), _max(max), _value(value) {
    if (_value < _min) {
      _value = _min;
    }
    if (_value > _max) {
      _value = _max;
    }
  }

  inline BoundedValue& operator=(Type value) {
    if (value < _min) {
      _value = _min;
    } else if (value > _max) {
      _value = _max;
    } else {
      _value = value;
    }
    return *this;
  }

  inline operator Type() const { return _value; }

 private:
  Type _min;
  Type _max;
  Type _value;
};

class UnitFloat {
  BoundedValue<float> _value;

 public:
  static constexpr float MIN_VALUE = 0.0f;
  static constexpr float MAX_VALUE = 1.0f;

  UnitFloat() : _value(MIN_VALUE, MAX_VALUE) {}
  UnitFloat(float value) : _value(MIN_VALUE, MAX_VALUE, value) {}

  inline UnitFloat& operator=(const float value) {
    _value = value;
    return *this;
  }

  inline operator float() const { return _value; }
};

}  // namespace crayon