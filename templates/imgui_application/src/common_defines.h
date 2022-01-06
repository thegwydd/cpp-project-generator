#pragma once

#include <memory>


#if defined(__GNUC__) || defined(__clang__)
#   define FUNCTION_NAME        __func__
#elif defined(_MSC_VER)
#   define FUNCTION_NAME        __FUNCTION__
#else
#   define FUNCTION_NAME        __FUNCTION__
#endif

#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(P) { (P) = (P); } 
#endif

#define SHARED_PTR(class_name)                  using sptr = std::shared_ptr<class_name>;
#define SHARED_CONST_PTR(class_name)            using csptr = std::shared_ptr<const class_name>;
#define FWD_DECL_SHARED_PTR(class_name)         using class_name##_sptr = std::shared_ptr<class_name>;
#define FWD_DECL_SHARED_CONST_PTR(class_name)   using class_name##_csptr = std::shared_ptr<const class_name>;

#define UNIQUE_PTR(class_name)                  using uptr = std::unique_ptr<class_name>;
#define FWD_DECL_UNIQUE_PTR(class_name)         using class_name##_uptr = std::unique_ptr<class_name>;

#define WEAK_PTR(class_name)                    using wptr = std::weak_ptr<class_name>;
#define FWD_DECL_WEAK_PTR(class_name)           using class_name##_wptr = std::weak_ptr<class_name>;

#define PTR(class_name)                         using ptr = class_name *;
#define FWD_DECL_PTR(class_name)                using class_name##_ptr = class_name *;

#define CONST_PTR(class_name)                   using cptr = const class_name *;
#define FWD_DECL_CONST_PTR(class_name)          using class_name##_cptr = const class_name *;

#define CONST_REF(class_name)                   using cref = const class_name &;
#define FWD_DECL_CONST_REF(class_name)          using class_name##_cref = const class_name &;

#define REF(class_name)                         using ref = class_name &;
#define FWD_DECL_REF(class_name)                using class_name##_ref = class_name &;

#define RVAL(class_name)                        using rval = class_name &&;
#define FWD_DECL_RVAL(class_name)               using class_name##_rval = class_name &&;


#define POINTER_TYPEDEFS(class_name)            SHARED_PTR(class_name)               \
                                                SHARED_CONST_PTR(class_name)         \
                                                UNIQUE_PTR(class_name)               \
                                                WEAK_PTR(class_name)                 \
                                                PTR(class_name)                      \
                                                CONST_PTR(class_name)                \
                                                CONST_REF(class_name)                \
                                                REF(class_name)                      \
                                                RVAL(class_name)

#define FWD_POINTER_TYPEDEFS(class_name)        FWD_DECL_SHARED_PTR(class_name)      \
                                                FWD_DECL_SHARED_CONST_PTR(class_name)\
                                                FWD_DECL_UNIQUE_PTR(class_name)      \
                                                FWD_DECL_WEAK_PTR(class_name)        \
                                                FWD_DECL_PTR(class_name)             \
                                                FWD_DECL_CONST_PTR(class_name)       \
                                                FWD_DECL_CONST_REF(class_name)       \
                                                FWD_DECL_REF(class_name)             \
                                                FWD_DECL_RVAL(class_name)


//////////////////////////////////////////////////////////////////////////
template <class InitFunctionT, class DeinitFunctionT>
struct raii_init_deinit_wrapper {
    raii_init_deinit_wrapper(InitFunctionT init, DeinitFunctionT deinit) : _deinit_fn(deinit) { init(); }
    ~raii_init_deinit_wrapper() { _deinit_fn(); }
    DeinitFunctionT _deinit_fn;
    };
