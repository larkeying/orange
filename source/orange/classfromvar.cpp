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


#include "vars.hpp"
#include "domain.hpp"
#include "examples.hpp"
#include "examplegen.hpp"
#include "transval.hpp"

#include "classfromvar.ppp"


inline TValue processValue(PTransformValue &transformer, const TValue &val, const PDistribution &distributionForUnknown)
{
  if (val.isSpecial()) {
    if (distributionForUnknown) {
      PDistribution distr = CLONE(TDistribution, distributionForUnknown);
      distr->normalize();
      return TValue(distr, val.varType, val.valueType);
    }
    else
      return val;
  }
  if (transformer)
    return transformer->call(val);
  else
    return val;
}


TClassifierFromVar::TClassifierFromVar(PVariable acv, PDistribution dun)
: TClassifier(acv),
  whichVar(acv),
  distributionForUnknown(dun)
{}


TClassifierFromVar::TClassifierFromVar(PVariable acv, PVariable awhichVar, PDistribution dun)
: TClassifier(acv),
  whichVar(awhichVar),
  distributionForUnknown(dun)
{}


TClassifierFromVar::TClassifierFromVar(const TClassifierFromVar &old)
: TClassifier(old),
  whichVar(old.whichVar),
  transformer(old.transformer),
  distributionForUnknown(old.distributionForUnknown)
{};


TValue TClassifierFromVar::operator ()(const TExample &example)
{ checkProperty(whichVar);

  TExample::const_iterator val = example.begin();

  TVarList::const_iterator vi(example.domain->variables->begin()), ei(example.domain->variables->end());
  for(; (vi!=ei) && (*vi!=whichVar); vi++, val++);
  if (vi!=ei)
    return processValue(transformer, *val, distributionForUnknown);

  TMetaVector::const_iterator mi(example.domain->metas.begin()), me(example.domain->metas.end());
  for( ; (mi!=me) && ((*mi).variable!=whichVar); mi++);

  if (mi!=me)
    return processValue(transformer, example.meta[(*mi).id], distributionForUnknown);

  if (whichVar->getValueFrom)
    return processValue(transformer, whichVar->computeValue(example), distributionForUnknown);

  int varType;
  if (distributionForUnknown->variable)
    varType = distributionForUnknown->variable->varType;
  else if (classVar)
    varType = classVar->varType;
  else if (distributionForUnknown->supportsDiscrete)
    varType = TValue::INTVAR;
  else if (distributionForUnknown->supportsContinuous)
    varType = TValue::FLOATVAR;
  else
    varType = TValue::OTHERVAR;
    
  return TValue(CLONE(TDistribution, distributionForUnknown), varType, valueDK);
}



TClassifierFromVarFD::TClassifierFromVarFD(PVariable acv, PDomain dom, const int &p, PDistribution dun, PTransformValue atrans)
: TClassifierFD(dom),
  position(p),
  transformer(atrans),
  distributionForUnknown(dun)
{ classVar = acv; }


TClassifierFromVarFD::TClassifierFromVarFD(const TClassifierFromVarFD &old)
: TClassifierFD(old),
  position(old.position),
  transformer(old.transformer),
  distributionForUnknown(old.distributionForUnknown)
{};


TValue TClassifierFromVarFD::operator ()(const TExample &example)
{ if (example.domain!=domain)
    raiseError("wrong domain");
  if (position==ILLEGAL_INT)
    raiseError("'position' not set");
  
  return processValue(transformer, example[position], distributionForUnknown);
}



TClassifierFromMeta::TClassifierFromMeta(PVariable acv, const int &ID, PDistribution dun)
: TClassifier(acv),
  whichID(ID),
  distributionForUnknown(dun)
{}


TClassifierFromMeta::TClassifierFromMeta(const TClassifierFromMeta &old)
: TClassifier(old),
  whichID(old.whichID),
  transformer(old.transformer),
  distributionForUnknown(old.distributionForUnknown)
{}


TValue TClassifierFromMeta::operator ()(const TExample &example)
{ checkProperty(whichID);

  return processValue(transformer, example.meta[whichID], distributionForUnknown);
}
