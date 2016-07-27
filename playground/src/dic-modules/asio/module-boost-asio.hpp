
#include <boost/dll/alias.hpp>

# include "di/container.hpp"
# include "asio/IoService.hpp"

TL_NS_BEGIN
  namespace module {
    namespace asio {

      namespace di = TLNS::di;

      /**
       * todo: FABIC_SYMBOL_EXPORT ?
       */
      class BoostAsioService
          : public di::definition< IoService >,
            public di::threaded_service_intf
      {
      private:

      public:
        /// Ctor
        explicit BoostAsioService();

        ~BoostAsioService();

        void construct() throw(di::base_exception) override;

        bool is_startable() const override { return true; }

        bool start() override;
        bool stop() override;

        /**
         * This _is_ the _“ entry point ”_ of this module (`.so` library),
         * which is “exported” as C-style symbole `di_register_services`
         * from `symfony/module/asio/module-boost-asio.cpp` :
         *
         *     BOOST_DLL_ALIAS(
         *         TLNS::module::asio::BoostAsioService::__di_register_services,
         *         di_register_services
         *     )
         */
        static void
        __di_register_services(di::container_shared_ptr_t container);

      };

    }
  }
TL_NS_END
