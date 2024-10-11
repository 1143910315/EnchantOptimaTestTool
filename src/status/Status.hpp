#pragma once
template<typename T>
class Status {
private:
    T data;
    bool isValid;
public:
    Status(T data);
    ~Status();
    const T& operator->() const;
    bool getValid() const;
    void setValid(bool valid);
    const T& getData() const;
};

template<typename T>
inline Status<T>::Status(T data) : data(data), isValid(true) {
}

template<typename T>
inline Status<T>::~Status() {
}

template<typename T>
inline const T& Status<T>::operator->() const {
    return data;
}

template<typename T>
inline bool Status<T>::getValid() const {
    return isValid;
}

template<typename T>
inline void Status<T>::setValid(bool valid) {
    isValid = valid;
}

template<typename T>
inline const T& Status<T>::getData() const {
    return data;
}
