
#include <memory>
#include <thread>

#include <boost/dll/alias.hpp>
#include <boost/network/protocol/http/server.hpp>

# include "fabic/logging.hpp"
# include "fabic/di/container.hpp"
# include "fabic/http/handler_functor.hpp"

TL_NS_BEGIN
  namespace module {
    namespace http {

      namespace netlib = boost::network::http;

      // Forward decl. for the below typedef `bserver_t`.
      class http_server;

      typedef std::shared_ptr<http_server> http_server_ptr_t;

      /**
       * “Boost cpp-netlib `boost::network::http::server<class Handler>`”
       */
      typedef netlib::server<http_server>            netlib_http_server_t;
      typedef std::shared_ptr<netlib_http_server_t>  netlib_http_server_ptr_t;


      //class server : public netlib::server<handler_functor> {
      // ^ not easy to extend this, actually...


      /**
       * Wrapper around the Boost cpp-netlib server thing.
       */
      class http_server
          : public std::enable_shared_from_this<http_server>
      {
      private:
        netlib_http_server_t::options  options_;
        netlib_http_server_ptr_t       server_;

      public:
        /// Ctor
        explicit http_server();

        /**
         * This _is_ the _“ entry point ”_ of this module (`.so` library),
         * which is “exported” as C-style symbole `di_register_services`
         * from `symfony/module/http/module-http-server.cpp` :
         *
         *     BOOST_DLL_ALIAS(
         *         TLNS::module::http::http_server::__di_register_services,
         *         di_register_services
         *     )
         */
        static void
        __di_register_services(di::container_shared_ptr_t container);

        /**
         * This is the function that handles the incoming request.
         *
         * @param request
         * @param connection
         */
        void operator()(
            netlib_http_server_t::request const &request,
            netlib_http_server_t::connection_ptr connection
        );

        netlib_http_server_t::options& options() { return this->options_; }

        // fixme temp: move impl to .cpp
        void run() {
          std::thread t1([&server = *this] { server.run(); });
          //server_->run();
        }
      };

    }
  }
TL_NS_END
