#include <iostream>
#include <typeinfo>
#include "shifres.hpp"

using namespace std;

enum class Type {SHAMIR=1, DIFFI_HELLMAN, RSA, EL_GHAMAL, CAESAR, ATBASH, GRONSFIELD, TABLET};

template <typename Enumeration>
auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type {
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
template<>
constexpr auto as_integer<Type>(Type const value) -> typename std::underlying_type<Type>::type {
    int n = static_cast<typename std::underlying_type<Type>::type>(value);
    if (n < 1 || n > 8)
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
            "5 - Caesar\n"
            "6 - Atbash\n"
            "7 - Gronsfield\n"
            "8 - By table\n"
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
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
            break;
        }
        case Type::SHAMIR: {
            Shamir session;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
            break;
        }
        case Type::RSA: {
            RSA session;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
            break;
        }
        case Type::CAESAR: {
            Caesar session;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
            break;
        }
        case Type::ATBASH: {
            Atbash session;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
            break;
        }
        case Type::GRONSFIELD: {
            Gronsfield session;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
            break;
        }
        case Type::TABLET: {
            Tablet session;
            cout << "processing..." << endl;
            session.recipient_protocol();
            cout << "Got message! (output.txt)" << endl;
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
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
            break;
        }
        case Type::SHAMIR: {
            Shamir session;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
            break;
        }
        case Type::RSA: {
            RSA session;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
            break;
        }
        case Type::CAESAR: {
            Caesar session;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
            break;
        }
        case Type::ATBASH: {
            Atbash session;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
            break;
        }
        case Type::GRONSFIELD: {
            Gronsfield session;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
            break;
        }
        case Type::TABLET: {
            Tablet session;
            cout << "processing..." << endl;
            session.dispatcher_protocol();
            cout << "Message input.txt was sent" << endl;
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

int main(int argc, char*argv[]) {
    if(argc==1) {
        Type type = chooseEncode();
        chooseMode(type);
        system("pause");
    } else {
        Environment &Environment = Environment::Instance();
        Type type = static_cast<Type>(0);
        void (*f)(Type) = nullptr;
        for(int i = 1; i < argc; i++) {
            if(argv[i][0]!='-') throw std::runtime_error("Incorrect input\n");
            else {
                switch(argv[i][1]) {
                    case 'i':
                        Environment.input = string(argv[++i]);
                        break;
                    case 'o':
                        Environment.output = string(argv[++i]);
                        break;
                    case 'b':
                        Environment.enableBinary = true;
                        break;
                    case '-': {
                        string str(argv[i]+2);
                        if(str==string("diffi-hellman")) type = Type::DIFFI_HELLMAN;
                        else if (str==string("shamir"))  type = Type::SHAMIR;
                        else if (str==string("el-ghamal"))  type = Type::EL_GHAMAL;
                        else if (str==string("rsa"))  type = Type::RSA;
                        else if (str==string("caesar"))  type = Type::CAESAR;
                        else if (str==string("atbash"))  type = Type::ATBASH;
                        else if (str==string("gronsfield"))  type = Type::GRONSFIELD;
                        else if (str==string("tablet"))  type = Type::TABLET;
                        else throw std::runtime_error("Incorrect input\n");
                        break;
                    }
                    case 'r':
                        f = emulateRecipient;
                        break;
                    case 'd':
                        f = emulateDispatcher;
                        break;
                }
            }
        }
        if(f == nullptr || type == static_cast<Type>(0)) throw std::runtime_error("Incorrect input\n");
        f(type);
    }
    return 0;
}
