/************************************************************************
**
**  Copyright (C) 2010  Strahinja Markovic
**
**  This file is part of FlightCrew.
**
**  FlightCrew is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlightCrew is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public License
**  along with FlightCrew.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#include "XmlUtils.h"
#include "ToXercesStringConverter.h"
#include "FromXercesStringConverter.h"
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <boost/foreach.hpp> 
#define foreach BOOST_FOREACH

extern const char *LOCATION_INFO_KEY;

typedef unsigned int uint;

namespace XercesExt
{

NodeLocationInfo GetNodeLocationInfo( const xc::DOMNode &node )
{
    return *static_cast< NodeLocationInfo* >( node.getUserData( toX( LOCATION_INFO_KEY ) ) );
}


std::vector< xc::DOMElement* > GetElementChildren( const xc::DOMElement &element )
{
    xc::DOMElement *child = element.getFirstElementChild();

    std::vector< xc::DOMElement* > children;

    if ( !child )

        return children;

    children.push_back( child );

    while (true)
    {
         child = child->getNextElementSibling();

         if ( !child )

             return children;
         
         children.push_back( child );
    }
}


bool ElementListContains( std::vector< xc::DOMElement* > element_list,
                          const QName &element_qname )
{
    for ( uint i = 0; i < element_list.size(); ++i )
    {
        xc::DOMElement *element = element_list[ i ];
        QName current_qname( fromX( element->getLocalName() ), 
                             fromX( element->getNamespaceURI() ) );

        if ( current_qname == element_qname )
        
            return true;
    } 

    return false;
}

xc::DOMNode* GetFirstAvailableElement( const std::vector< QName > &element_qnames,
                                       const xc::DOMDocument &document )
{
    foreach( QName element_qname, element_qnames )
    {
        xc::DOMNodeList *matching_nodes = document.getElementsByTagNameNS(
               toX( element_qname.namespace_name ),  toX( element_qname.local_name ) );

        if ( matching_nodes->getLength() > 0 )

            return matching_nodes->item( 0 );
    }

    return NULL;
}

xc::DOMNode* GetFirstAvailableElement( const QName &element_qname, 
                                       const xc::DOMDocument &document )
{
    std::vector< QName > element_qnames;
    element_qnames.push_back( element_qname );

    return GetFirstAvailableElement( element_qnames, document );
}

} // namespace XercesExt
