#ifndef FABIC_DI_SERVICE_DEFINITION
#define FABIC_DI_SERVICE_DEFINITION

#include <map>
#include <string>

namespace fabic {
    namespace di {

        using std::string;
        using std::map;
        using std::pair;

        class ServiceDefinition {
            typedef map<string, string> arguments_t;
            typedef void (*ServiceCtorFunc_t)(arguments_t);
        private:
            ServiceCtorFunc_t constructService = nullptr;
            arguments_t arguments;
        public:
            ServiceDefinition(ServiceCtorFunc_t ctor) : constructService(ctor) {}
        };

    }
}

#endif