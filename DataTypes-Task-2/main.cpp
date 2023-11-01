#include <iostream>
#include <typeinfo>
#include <stdexcept>

class AnyType {
public:
    AnyType() : data(nullptr), type(nullptr) {}

    template<typename T>
    AnyType(const T& value) : data(new DataImpl<T>(value)), type(&typeid(T)) {}

    AnyType(const AnyType& other) : data(other.data ? other.data->clone() : nullptr), type(other.type) {}

    // Assignment operator for deep copying another AnyType object.
    AnyType& operator=(const AnyType& other) {
        if (this != &other) {
            delete data;
            data = other.data ? other.data->clone() : nullptr;
            type = other.type;
        }
        return *this;
    }

    ~AnyType() {
        delete data;
    }

    template<typename T>
    AnyType& operator=(const T& value) {
        delete data;
        data = new DataImpl<T>(value);
        type = &typeid(T);
        return *this;
    }

    void swap(AnyType& other) {
        std::swap(data, other.data);
        std::swap(type, other.type);
    }

    void destroy() {
        delete data;
        data = nullptr;
        type = nullptr;
    }

    template<typename T>
    T& getValue() const {
        if (type != &typeid(T)) {
            throw std::bad_cast();
        }
        return static_cast<DataImpl<T>*>(data)->data;
    }

#pragma region OperatorOverloading
    // Overload equality operator (==) to compare two AnyType objects.
    bool operator==(const AnyType& other) const {
        if (type != other.type) {
            return false; // Different types are not equal.
        }
        // Use dynamic_cast to check equality of the stored data.
        const auto* thisData = dynamic_cast<const DataImplBase*>(data);
        const auto* otherData = dynamic_cast<const DataImplBase*>(other.data);
        return thisData && otherData && thisData->equals(*otherData);
    }

    // Overload inequality operator (!=) to compare two AnyType objects.
    bool operator!=(const AnyType& other) const {
        return !(*this == other); // Inequality is the negation of equality.
    }
#pragma endregion


private:
    // Base class for storing data polymorphically.
    class Data {
    public:
        virtual ~Data() {}
        virtual Data* clone() const = 0;
    };

#pragma region OperatorOverloading
    // Base class for storing data polymorphically.
    class DataImplBase : public Data {
    public:
        virtual ~DataImplBase() {}
        virtual bool equals(const DataImplBase& other) const = 0; // Pure virtual function for equality check.
    };

    // Derived class template for storing specific data type and implementing equals method.
    template<typename T>
    class DataImpl : public DataImplBase {
    public:
        DataImpl(const T& value) : data(value) {}

        virtual Data* clone() const override {
            return new DataImpl<T>(data);
        }

        virtual bool equals(const DataImplBase& other) const override {
            // Dynamic cast to check the type, then compare the stored data.
            if (auto* otherData = dynamic_cast<const DataImpl<T>*>(&other)) {
                return data == otherData->data;
            }
            return false;
        }

        T data;
    };
#pragma endregion

    Data* data;
    const std::type_info* type;
};

int main() {
    try {
        AnyType anyType = 1;
        anyType = true;
        anyType = 1.7;

        try {
            int storedValue = anyType.getValue<int>();
            std::cout << "Stored int value: " << storedValue << std::endl;
        }
        catch (const std::bad_cast& e) {
            std::cerr << "Bad cast exception: " << e.what() << std::endl;
        }

        double storedValue = anyType.getValue<double>();
        std::cout << "Stored double value: " << storedValue << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    AnyType a = 42;
    AnyType b = 42;
    AnyType c = 3.14;

    std::cout << "a == b: " << (a == b) << std::endl;
    std::cout << "a == c: " << (a == c) << std::endl;

    return 0;
}
