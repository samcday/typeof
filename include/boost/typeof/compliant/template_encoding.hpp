// Copyright (C) 2004 Arkadiy Vertleyb
// Copyright (C) 2005 Peder Holt 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_COMPLIANT_TEMPLATE_ENCODING_HPP_INCLUDED
#define BOOST_TYPEOF_COMPLIANT_TEMPLATE_ENCODING_HPP_INCLUDED

#include <boost/typeof/compliant/encode_decode.hpp>
#include <boost/typeof/compliant/int_encoding.hpp>

#include <boost/typeof/template_encoding.hpp>

#include <boost/typeof/compliant/type_template_param.hpp>
#include <boost/typeof/compliant/integral_template_param.hpp>
#include <boost/typeof/compliant/template_template_param.hpp>

//////////

// "function objects"

#define BOOST_TYPEOF_REGISTER_TEMPLATE_ENCODE_PARAM(r, data, n, elem)\
    BOOST_TYPEOF_PARAM_ENCODE(BOOST_TYPEOF_MAKE_OBJ(elem))(BOOST_TYPEOF_MAKE_OBJ(elem), n)

#define BOOST_TYPEOF_REGISTER_TEMPLATE_DECODE_PARAM(r, data, n, elem)\
    BOOST_TYPEOF_PARAM_DECODE(BOOST_TYPEOF_MAKE_OBJ(elem))(BOOST_TYPEOF_MAKE_OBJ(elem), n)

#define BOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_PAIR(z,n,elem) \
    BOOST_TYPEOF_EXPAND_TYPE(BOOST_TYPEOF_MAKE_OBJ(elem)) BOOST_PP_CAT(P, n)

//Branch the final typedef between the 'normal' typedef and template template typedef
#define BOOST_TYPEOF_REGISTER_TEMPLATE_TYPEDEF_TYPE(Name,Params,ID)\
    BOOST_PP_IF(BOOST_TYPEOF_HAS_TEMPLATE_TEMPLATE_ARGUMENTS(Params),\
        BOOST_TYPEOF_REGISTER_TEMPLATE_TEMPLATE_TYPE,\
        BOOST_TYPEOF_REGISTER_DEFAULT_TEMPLATE_TYPE\
    )(Name,Params,ID)

#define BOOST_TYPEOF_REGISTER_DEFAULT_TEMPLATE_TYPE(Name,Params,ID)\
    Name< BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(Params), P) >


#define BOOST_TYPEOF_REGISTER_TEMPLATE_X_IMPL(Name, Params, ID)\
    BOOST_TYPEOF_REGISTER_ARGUMENTS_FOR_TT_ENCODING(Name, Params, ID)\
    template<class V\
        BOOST_TYPEOF_SEQ_ENUM_TRAILING(Params,BOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_PAIR)\
    >\
    struct encode_type_impl<V, Name<\
        BOOST_PP_ENUM_PARAMS(\
        BOOST_PP_SEQ_SIZE(Params),\
        P)> >\
    {\
        typedef typename BOOST_TYPEOF_PUSH_BACK<V, boost::mpl::size_t<ID> >::type V0;\
        BOOST_PP_SEQ_FOR_EACH_I(\
            BOOST_TYPEOF_REGISTER_TEMPLATE_ENCODE_PARAM,\
            ~,\
            Params)\
        typedef BOOST_PP_CAT(V, BOOST_PP_SEQ_SIZE(Params)) type;\
    };\
    template<class Iter>\
    struct decode_type_impl<boost::mpl::size_t<ID>, Iter>\
    {\
        typedef Iter iter0;\
        BOOST_PP_SEQ_FOR_EACH_I(\
            BOOST_TYPEOF_REGISTER_TEMPLATE_DECODE_PARAM,\
            ~,\
            Params)\
        typedef BOOST_TYPEOF_REGISTER_TEMPLATE_TYPEDEF_TYPE(Name,Params,ID) type;\
        typedef BOOST_PP_CAT(iter, BOOST_PP_SEQ_SIZE(Params)) iter;\
    };

#define BOOST_TYPEOF_REGISTER_TEMPLATE_X(Name, Params)\
    namespace boost{namespace type_of{namespace{\
    BOOST_TYPEOF_REGISTER_TEMPLATE_X_IMPL(Name, Params, BOOST_TYPEOF_UNIQUE_ID())\
    }}}

#endif//BOOST_TYPEOF_COMPLIANT_TEMPLATE_ENCODING_HPP_INCLUDED
