#include <pmtv/rva_variant.hpp>
#include <pmtv/type_helpers.hpp>
#include <pmtv/version.hpp>
#include "base64/base64.h"
#include <cstddef>
#include <complex>
#include <ranges>
#include <span>

namespace pmtv {


using pmt = rva::variant<
    std::nullptr_t,
    uint8_t, uint16_t, uint32_t, uint64_t,
    int8_t, int16_t, int32_t, int64_t,
    float, double, std::complex<float>, std::complex<double>,
    std::vector<uint8_t>, std::vector<uint16_t>, std::vector<uint32_t>, std::vector<uint64_t>,
    std::vector<int8_t>, std::vector<int16_t>, std::vector<int32_t>, std::vector<int64_t>,
    std::vector<float>, std::vector<double>,
    std::vector<std::complex<float>>, std::vector<std::complex<double>>,
    std::string,
    std::vector<rva::self_t>,
    std::map<std::string, rva::self_t>>;

template <class T>
inline constexpr std::in_place_type_t<std::vector<T>> vec_t{};

template <typename T>
concept IsPmt = std::is_same_v<T, pmt>;

/*template <class T, class V>
auto get_vector(V value) -> decltype(std::get<std::vector<T>>(value) {
    return std::get<std::vector<T>>(value);
}*/

template <class T, class V>
std::span<T> get_span(V& value) {
    return std::span(std::get<std::vector<T>>(value));
}

template <std::ranges::view T>
std::ostream& _ostream_pmt_vector(std::ostream& os, const T& vec) {
    bool first = true;
    os << "[";
    for (const auto& v: vec) {
        if (first) os << v;
        else os << ", " << v;
        first = false;
    }
    os << "]";
    return os;
}

template <IsPmt P>
std::ostream& operator<<(std::ostream& os, const P& value) {
    return std::visit([&os](const auto& arg) -> std::ostream& {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(Complex<T>) os << "(" << arg.real() << "," << arg.imag() << ")";
        else if constexpr(Scalar<T>) os << arg;
        else if constexpr(UniformVector<T>)  _ostream_pmt_vector(os, std::span(arg));
        /*else if constexpr(IsSharedPtr<T>) {
            if constexpr(UniformVector<typename T::element_type>) {
                _ostream_pmt_vector(os, std::span(*arg));
            } else if constexpr(PmtVector<typename T::element_type>) {
                //_ostream_pmt_vector(os, std::span(*arg));
            } else if constexpr(PmtMap<typename T::element_type>) {
                //_ostream_pmt_map(os, *arg);
            }
        }*/
        else if constexpr(std::same_as<std::nullptr_t, T>) os << "null";
        else if constexpr(std::same_as<T, std::string>) os << arg;
        return os; }
        , value.get_base());
}

template <IsPmt P>
size_t elements(const P& value) {
    return std::visit([](const auto& arg) -> size_t {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::same_as<std::nullptr_t, T>) return 0;
        else if constexpr(std::ranges::range<T>) return arg.size();
        return 1; }
        , value.get_base());
}

template <IsPmt P>
size_t bytes_per_element(const P& value) {
    return std::visit([](const auto& arg) -> size_t {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::same_as<std::nullptr_t, T>) return 0;
        else if constexpr(std::ranges::range<T>) return sizeof(typename T::value_type);
        return sizeof(T); }
        , value.get_base());
}
// Can move to library function
template <IsPmt P>
size_t serialize(std::streambuf& sb, const P& value) {
    size_t length = 0;
    length += sb.sputn(reinterpret_cast<const char*>(&pmt_version), 2);
    uint16_t index = value.index();
    length += sb.sputn(reinterpret_cast<const char*>(&index), 2);
    length += std::visit([&sb](auto&& arg) -> size_t {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::same_as<T, std::nullptr_t>) return 0;
        else if constexpr(Scalar<T>) {
            return sb.sputn(reinterpret_cast<const char*>(&arg), sizeof(arg));
        } else if constexpr(std::ranges::contiguous_range<T> && Scalar<typename T::value_type>) {
            return sb.sputn(reinterpret_cast<const char*>(arg.data()), arg.size()*sizeof(arg));
        } else if constexpr(std::same_as<T, std::vector<pmt>>) {
            size_t length = 0;
            for (const auto& elem: arg) length += serialize(sb, elem);
            return length;
        } else return 0;
        
    }, value.get_base());
    std::cout << "Serialize: " << length << " " << index << std::endl;
    /*pmt_container_type container;
    std::visit([&container](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        container = container_type<T>();        
    }, value);

    length += sb.sputn(reinterpret_cast<const char*>(&container), 2);

    if (container == pmt_container_type::EMPTY) {
        // do nothing
    }
    else if (container == pmt_container_type::SCALAR) {

        std::visit([&length, &sb](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            auto id = element_type<T>();
            length += sb.sputn(reinterpret_cast<const char*>(&id), 1);
            auto v = arg;
            length += sb.sputn(reinterpret_cast<const char*>(&v), sizeof(v));
            
        }, value);
    
    }
    else if (container == pmt_container_type::UNIFORM_VECTOR) {
        std::visit([&length, &sb](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr(UniformVectorInsidePmt<T>) {
                auto id = element_type<typename T::element_type>();
                length += sb.sputn(reinterpret_cast<const char*>(&id), 1);
                uint64_t sz = arg->size();
                length += sb.sputn(reinterpret_cast<const char*>(&sz), sizeof(uint64_t));
                length += sb.sputn(reinterpret_cast<const char*>(arg->data()), arg->size()*sizeof((*arg)[0]));
            }
        }, value);
    
    }*/

    return length;
}

template <Scalar T>
T deserialize_val(std::streambuf& sb) {
    T val;
    sb.sgetn(reinterpret_cast<char*>(&val), sizeof(val));
    std::cout << val << std::endl;
    return val;
}
    

inline pmt deserialize(std::streambuf& sb)
{
    uint16_t version;
    uint16_t index;
    sb.sgetn(reinterpret_cast<char*>(&version), 2);
    sb.sgetn(reinterpret_cast<char*>(&index), 2);
    std::cout << "Deserial: " << version << " " << int(index) << std::endl;
    /*for (size_t i = 0; i < std::variant_size_v<pmt>; i++) {
        //using T = std::variant_alternative_t<i, pmt>;
        if constexpr(Scalar<std::variant_alternative_t<i>) {
            if (i == index) return deserialize_val<std::variant_alternative_t<i>(sb);
        } else if constexpr(
        
    }*/
    /*switch(container) {
        case pmt_element_type::UINT8: return deserialize_val<uint8_t>(sb);
        case pmt_element_type::UINT16: return deserialize_val<uint16_t>(sb);
        case pmt_element_type::UINT32: return deserialize_val<uint32_t>(sb);
        case pmt_element_type::UINT64: return deserialize_val<uint64_t>(sb);
        case pmt_element_type::INT8: return deserialize_val<int8_t>(sb);
        case pmt_element_type::INT16: return deserialize_val<int16_t>(sb);
        case pmt_element_type::INT32: return deserialize_val<int32_t>(sb);
        case pmt_element_type::INT64: return deserialize_val<int64_t>(sb);
        case pmt_element_type::FLOAT: return deserialize_val<float>(sb);
        case pmt_element_type::DOUBLE: return deserialize_val<double>(sb);
        case pmt_element_type::COMPLEX_FLOAT: return deserialize_val<std::complex<float>>(sb);
        case pmt_element_type::COMPLEX_DOUBLE: return deserialize_val<std::complex<double>>(sb);
        default: return pmt();
        }*/

    /*pmt ret;
    if (container == pmt_container_type::EMPTY) {
        // do nothing
    }
    else if (container == pmt_container_type::SCALAR) {
        pmt_element_type T_type;
        //sb.sgetn(reinterpret_cast<char*>(&T_type), sizeof(T_type));

        switch(container) {
            case pmt_element_type::UINT8: return deserialize_val<uint8_t>(sb);
            case pmt_element_type::UINT16: return deserialize_val<uint16_t>(sb);
            case pmt_element_type::UINT32: return deserialize_val<uint32_t>(sb);
            case pmt_element_type::UINT64: return deserialize_val<uint64_t>(sb);
            case pmt_element_type::INT8: return deserialize_val<int8_t>(sb);
            case pmt_element_type::INT16: return deserialize_val<int16_t>(sb);
            case pmt_element_type::INT32: return deserialize_val<int32_t>(sb);
            case pmt_element_type::INT64: return deserialize_val<int64_t>(sb);
            case pmt_element_type::FLOAT: return deserialize_val<float>(sb);
            case pmt_element_type::DOUBLE: return deserialize_val<double>(sb);
            case pmt_element_type::COMPLEX_FLOAT: return deserialize_val<std::complex<float>>(sb);
            case pmt_element_type::COMPLEX_DOUBLE: return deserialize_val<std::complex<double>>(sb);
            default:{

            }
        }
    }
    else if (container == pmt_container_type::UNIFORM_VECTOR) {
        pmt_element_type T_type;
        sb.sgetn(reinterpret_cast<char*>(&T_type), 1);
        uint64_t sz; 
        sb.sgetn(reinterpret_cast<char*>(&sz), sizeof(uint64_t));

        switch(T_type) {
            case pmt_element_type::UINT8: {
                std::vector<uint8_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::UINT16: {
                std::vector<uint16_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::UINT32: {
                std::vector<uint32_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::UINT64: {
                std::vector<uint64_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::INT8: {
                std::vector<int8_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::INT16: {
                std::vector<int16_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::INT32: {
                std::vector<int32_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::INT64: {
                std::vector<int64_t> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::FLOAT: {
                std::vector<float> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::DOUBLE: {
                std::vector<double> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::COMPLEX_FLOAT: {
                std::vector<std::complex<float>> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            } break;
            case pmt_element_type::COMPLEX_DOUBLE: {
                std::vector<std::complex<double>> val(sz);
                sb.sgetn(reinterpret_cast<char*>(val.data()), sz*sizeof(val[0]));
                ret = pmt(val);
            }
            default:{

            }
        }
    }

    return ret;*/
}

template <IsPmt P>
std::string to_base64(const P& value)
{
    std::stringbuf sb; 
    auto nbytes = serialize(sb, value);
    std::string pre_encoded_str(nbytes, '0');
    sb.sgetn(pre_encoded_str.data(), nbytes);
    auto nencoded_bytes = Base64encode_len(nbytes);
    std::string encoded_str(nencoded_bytes, '0');
    auto nencoded = Base64encode(encoded_str.data(), pre_encoded_str.data(), nbytes);
    encoded_str.resize(nencoded - 1); // because it null terminates
    return encoded_str;
}

inline pmt from_base64(const std::string& encoded_str)
{
    std::string bufplain(encoded_str.size(), '0');
    Base64decode(bufplain.data(), encoded_str.data());
    std::stringbuf sb(bufplain);
    return deserialize(sb); 
}
}