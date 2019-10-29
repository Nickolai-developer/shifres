#include <iostream>
#include <typeinfo>
#include "shifres.hpp"

using namespace std;

enum class Type {SHAMIR=1, DIFFI_HELLMAN, RSA, EL_GHAMAL};

template <typename Enumeration>
auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type {
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
template<>
constexpr auto as_integer<Type>(Type const value) -> typename std::underlying_type<Type>::type {
    int n = static_cast<typename std::underlying_type<Type>::type>(value);
    if (n < 1 || n > 4)
        throw std::runtime_error("Incorrect input");
    return n;
}

Type chooseEncode() {
    int t;
    inputerror:
    cout << "1 - Shamir`s\n"
            "2 - Diffi-Hellman`s\n"
            "3 - RSA\n"
            "4 - El-Ghamal`s\n"
            "Enter type of simulated encryption algorythm: ";
    cin >> t;
    cout << endl;
    try {
        Type encode = static_cast<Type>(t);
        cout << as_integer(encode) << endl;
        return encode;
    }
    catch (std::exception &e) {
        cerr << e.what() << endl;
        goto inputerror;
    }
}

void emulateRecipient(Type type) {
    init(".recipient", ".dispatcher"); /// this is a crutch. it shouldn't be here, but we have it
    switch (type) {
        case Type::DIFFI_HELLMAN: {
            Diffi_Hellman session;
            cout << "\nMy shared key is " << session.sharedKey <<
                ". Just reporting, nothing more. I won`t tell you my secret key." << endl;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "\nI`m abonent B. My secret number is " << session.getEvaluatedNumber() <<
                    "; abonent A has equal secret number" << endl;
            break;
        }
        case Type::EL_GHAMAL: {
            El_Ghamal session;
            session.recipient_protocol();
            break;
        }
        case Type::SHAMIR: {
            Shamir session;
            session.recipient_protocol();
            break;
        }
        case Type::RSA: {
            RSA session;
            session.recipient_protocol();
            break;
        }
    }
}

void emulateDispatcher(Type type) {
    init(".dispatcher", ".recipient"); /// this is a crutch. it shouldn't be here, but we have it
    switch (type) {
        case Type::DIFFI_HELLMAN: {
            Diffi_Hellman session;
            cout << "\nMy shared key is " << session.sharedKey <<
                ". Just reporting, nothing more. I won`t tell you my secret key." << endl;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "\nI`m abonent A. My secret number is " << session.getEvaluatedNumber() <<
                    "; abonent B has equal secret number" << endl;
            break;
        }
        case Type::EL_GHAMAL: {
            El_Ghamal session;
            session.dispatcher_protocol();
            break;
        }
        case Type::SHAMIR: {
            Shamir session;
            session.dispatcher_protocol();
            break;
        }
        case Type::RSA: {
            RSA session;
            session.dispatcher_protocol();
            break;
        }
    }
}

void chooseMode(Type type) {
    int m;
    err:
    cout << "1 - Emulate recipient\n"
            "2 - Emulate dispatcher to encrypt file (shared key of any recipient required)\n"
            "Choose mode: ";
    cin >> m;
    cout << endl;
    switch(m) {
        case DISPATCHER:
            emulateDispatcher(type);
            break;
        case RECIPIENT:
            emulateRecipient(type);
            break;
        default:
            goto err;
    }
};

int main() {
    Type type = chooseEncode();
    chooseMode(type);
    system("pause"); //TODO remove
    return 0;
}
