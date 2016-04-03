// This file has been generated by Py++.

#include "boost/python.hpp"
#include "generators/include/python_CEGUI.h"
#include "WidgetComponentIterator.pypp.hpp"

namespace bp = boost::python;

struct ConstVectorIterator_less__CEGUI_scope_NamedDefinitionCollator_less__CEGUI_scope_String_comma__CEGUI_scope_WidgetComponent_const_ptr___greater___greater__wrapper : CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > >, bp::wrapper< CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > > > {

    ConstVectorIterator_less__CEGUI_scope_NamedDefinitionCollator_less__CEGUI_scope_String_comma__CEGUI_scope_WidgetComponent_const_ptr___greater___greater__wrapper(CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, CEGUI::WidgetComponent const*> > const & arg )
    : CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, CEGUI::WidgetComponent const*> >( arg )
      , bp::wrapper< CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > > >(){
        // copy constructor
        
    }

    ConstVectorIterator_less__CEGUI_scope_NamedDefinitionCollator_less__CEGUI_scope_String_comma__CEGUI_scope_WidgetComponent_const_ptr___greater___greater__wrapper( )
    : CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, CEGUI::WidgetComponent const*> >( )
      , bp::wrapper< CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > > >(){
        // null constructor
    
    }

    virtual ::CEGUI::WidgetComponent const * getCurrentValue(  ) const  {
        if( bp::override func_getCurrentValue = this->get_override( "getCurrentValue" ) )
            return func_getCurrentValue(  );
        else{
            return this->CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > >::getCurrentValue(  );
        }
    }
    
    ::CEGUI::WidgetComponent const * default_getCurrentValue(  ) const  {
        return CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > >::getCurrentValue( );
    }

};

void Iterator_next(::CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, CEGUI::WidgetComponent const*> >& t)
{
    t++;
}

void Iterator_previous(::CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, CEGUI::WidgetComponent const*> >& t)
{
    t--;
}

void register_WidgetComponentIterator_class(){

    { //::CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > >
        typedef bp::class_< ConstVectorIterator_less__CEGUI_scope_NamedDefinitionCollator_less__CEGUI_scope_String_comma__CEGUI_scope_WidgetComponent_const_ptr___greater___greater__wrapper, bp::bases< CEGUI::ConstBaseIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* >, CEGUI::WidgetComponent const* > > > WidgetComponentIterator_exposer_t;
        WidgetComponentIterator_exposer_t WidgetComponentIterator_exposer = WidgetComponentIterator_exposer_t( "WidgetComponentIterator", bp::no_init );
        bp::scope WidgetComponentIterator_scope( WidgetComponentIterator_exposer );
        WidgetComponentIterator_exposer.def( bp::init< >("*************************************************************************\n\
           No default construction available\n\
        *************************************************************************\n") );
        { //::CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > >::getCurrentValue
        
            typedef CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > > exported_class_t;
            typedef ::CEGUI::WidgetComponent const * ( exported_class_t::*getCurrentValue_function_type )(  ) const;
            typedef ::CEGUI::WidgetComponent const * ( ConstVectorIterator_less__CEGUI_scope_NamedDefinitionCollator_less__CEGUI_scope_String_comma__CEGUI_scope_WidgetComponent_const_ptr___greater___greater__wrapper::*default_getCurrentValue_function_type )(  ) const;
            
            WidgetComponentIterator_exposer.def( 
                "getCurrentValue"
                , getCurrentValue_function_type(&::CEGUI::ConstVectorIterator< CEGUI::NamedDefinitionCollator< CEGUI::String, CEGUI::WidgetComponent const* > >::getCurrentValue)
                , default_getCurrentValue_function_type(&ConstVectorIterator_less__CEGUI_scope_NamedDefinitionCollator_less__CEGUI_scope_String_comma__CEGUI_scope_WidgetComponent_const_ptr___greater___greater__wrapper::default_getCurrentValue)
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        WidgetComponentIterator_exposer.def("next", &::Iterator_next);
        WidgetComponentIterator_exposer.def("previous", &::Iterator_previous);
    }

}