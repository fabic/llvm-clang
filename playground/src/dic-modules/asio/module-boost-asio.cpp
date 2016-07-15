
# include "module-boost-asio.hpp"

TL_NS_BEGIN
  namespace module {
    namespace asio {

    namespace di = TLNS::di;

      // Ctor.
      BoostAsioService::BoostAsioService()
          : definition("boost.asio.io_service")
      {
      }


      BoostAsioService::~BoostAsioService() { }


      void
      BoostAsioService::construct()
        throw(di::base_exception) // override
      {
        if (this->has_instance())
          BOOST_THROW_EXCEPTION( di::service_already_constructed() );

        assert( this->factory == nullptr );

        this->instance = std::make_shared< IoService >();
      }


      // static btw.
      void
      BoostAsioService::__di_register_services(
          di::container_shared_ptr_t container
        )
      {
        logtrace << "HEY! that's module-boost-asio __di_register_services() ;- Hura!";

        auto serv = std::make_shared< BoostAsioService >();

        container->register_service(
            //std::make_shared< BoostAsioService >()
            serv
          );
      }


      bool
      BoostAsioService::start() // override btw.
      {
        logdebug << " » BoostAsioService::start(): begin.";

        io_service& ios_ = *this->instance;

        //ios_.run();

        logdebug << " » BoostAsioService::start(): end.";

        return false;
      }


      bool
      BoostAsioService::stop() // override btw.
      {
        return false;
      }


    }
  }
TL_NS_END


BOOST_DLL_ALIAS_SECTIONED(
    TLNS::module::asio::BoostAsioService::__di_register_services,
    module_asio_di_register_services,
    Anna
  )
