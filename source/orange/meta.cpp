/*
    This file is part of Orange.

    Orange is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Orange is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Orange; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Authors: Janez Demsar, Blaz Zupan, 1996--2002
    Contact: janez.demsar@fri.uni-lj.si
*/


#include "stladdon.hpp"
#include "vars.hpp"
#include "meta.hpp"


const char *_unknownweightexception = "example has undefined weight";
const char *_noncontinuousweightexception = "example has non-continuous weight";
const char *_getweightwho = "_getweight";

long metaID=1;


long getMetaID()
{ return ++metaID; }


TMetaDescriptor::TMetaDescriptor(const long &ai, const PVariable &avar)
: id(ai),
  variable(avar)
{}


TMetaDescriptor *TMetaVector::operator[](PVariable var)
{ this_ITERATE(mi)
    if ((*mi).variable==var)
      return &*mi;
  return (TMetaDescriptor *)NULL; 
}


TMetaDescriptor const *TMetaVector::operator[](PVariable var) const
{ const_this_ITERATE(mi)
    if ((*mi).variable==var)
      return &*mi;
  return (TMetaDescriptor const *)NULL; 
}


TMetaDescriptor *TMetaVector::operator[](const string &sna) 
{ this_ITERATE(mi)
    if ((*mi).variable->name==sna)
      return &*mi;
  return (TMetaDescriptor *)NULL; 
}


TMetaDescriptor const *TMetaVector::operator[](const string &sna) const
{ const_this_ITERATE(mi)
    if ((*mi).variable->name==sna)
      return &*mi;
  return (TMetaDescriptor const *)NULL; 
}


TMetaDescriptor *TMetaVector::operator[](const long &ai) 
{ this_ITERATE(mi)
    if ((*mi).id==ai)
      return &*mi;
  return (TMetaDescriptor *)NULL;
}


TMetaDescriptor const *TMetaVector::operator[](const long &ai) const
{ const_this_ITERATE(mi)
    if ((*mi).id==ai)
      return &*mi;
  return (TMetaDescriptor const *)NULL; 
}


TValue &TMetaValues::operator[](long id)
{ this_ITERATE(si)
    if ((*si).first==id)
      return (*si).second;

  raiseError("meta value with id %i not found", id);
  throw 0;
}


const TValue &TMetaValues::operator[](long id) const
{ const_this_ITERATE(si)
    if ((*si).first==id)
      return (*si).second;

  raiseError("meta value with id %i not found", id);
  throw 0;
}


bool TMetaValues::exists(long id) const
{ const_this_ITERATE(si)
    if ((*si).first==id)
      return true;
  return false;
}


void TMetaValues::setValue(const long &id, const TValue &val)
{ this_ITERATE(si)
    if ((*si).first==id) {
      *si = pair<long, TValue>(id, val);
      return;
    }

  push_back(pair<long, TValue>(id, val));
}


void TMetaValues::removeValue(const long &id)
{ this_ITERATE(si)
    if ((*si).first==id) {
      erase(si);
      return;
    }

  raiseError("meta value with id %i not found", id);
}

void TMetaValues::removeValueIfExists(const long &id)
{ this_ITERATE(si)
    if ((*si).first==id) {
      erase(si);
      return;
    }
}
