//
// Created by fabi on 6/18/16.
//

#ifndef MIXED_DATATYPE_OBJECT_HPP
#define MIXED_DATATYPE_OBJECT_HPP

  /** Base _dependency container manageable_ object class.
   *
   * for type checking `dynamic_cast<object>(...)` ability
   * and storage by the container.
   */
  class object {
  public:
      virtual ~object() {}
  };

#endif //MIXED_DATATYPE_OBJECT_HPP
