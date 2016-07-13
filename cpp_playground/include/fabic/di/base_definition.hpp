//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_BASE_SERVICE_DEFINITION_HPP
#define FABICCPPPLAYGROUND_BASE_SERVICE_DEFINITION_HPP


# include "fabic/di/dependency.hpp"

TL_NS_BEGIN
  namespace di {

    using std::string;
    using std::map;
    using std::pair;
    using std::shared_ptr;

    /**
     * Base abstract class for “ service definitions ”.
     *
     * todo: see http://stackoverflow.com/questions/16082785/use-of-enable-shared-from-this-with-multiple-inheritance
     *       for usage of enable_shared_from_this<> in context on inheritance
     *       and up/down-casting issues wrt. preserving "the same shared ptr group".
     */
    class FABIC_SYMBOL_EXPORT base_definition
        : public std::enable_shared_from_this<base_definition>
    {
    private:
        // Prevent client codes from having implicit copies.
        base_definition(const base_definition&) = delete;
        base_definition& operator=(const base_definition&) = delete;

    public:
        typedef shared_ptr<base_definition> pointer;
        typedef map<string, base_dependency_declaration *> dependencies_map;
        typedef typename boost::call_traits<dependencies_map>::reference dependencies_map_ref;
        typedef typename boost::call_traits<dependencies_map>::const_reference dependencies_map_cref;

    protected:
        string id_;
        // Depth-first post-order search
        bool dfs_resolving_ = false;
        bool dfs_visited_   = false;

    public:
        explicit base_definition(string name) : id_(name) {}

        virtual ~base_definition() {}

        /**
         * @return the service identifier / name.
         */
        string id() { return this->id_; }

        virtual const type_info& get_type_info() const {
            throw new std::exception();
        }

        // FIXME: temp.
        string get_service_definition_type_name() {
            return type_info::type_id_runtime(*this).pretty_name();
        }

        virtual dependencies_map_cref get_dependencies_map() const {
            throw new std::exception();
        }

        virtual bool has_instance() const =0;

        virtual void construct() =0;

        bool is_resolving_in_progress() const { return this->dfs_resolving_ ; }
        bool was_visited() const { return this->dfs_visited_ ; }

        void set_resolving_in_progress(bool b) { this->dfs_resolving_ = b ;}
        void set_visited(bool b) { this->dfs_visited_ = b ;}

        virtual bool is_startable() const =0;
        virtual bool start() =0;

      };

  } // di ns.
TL_NS_END
#endif //FABICCPPPLAYGROUND_BASE_SERVICE_DEFINITION_HPP
