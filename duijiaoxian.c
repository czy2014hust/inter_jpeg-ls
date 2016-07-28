
///*********first*************///
if((*vars).Rc >= max((*vars).Ra,(*vars).Rb))
{
  if(((*vars).Rc - max((*vars).Rb,(*vars).Ra)) > t1 && abs((*vars).Rb - (*vars).Ra) < t2)
    (*vars).Px = (*vars).Rb + (*vars).Ra - (*vars).Rd;
  else
    (*vars).Px = (*vars).Rb + (*vars).Ra - (*vars).Rc;
  if(abs((*vars).Rb - (*vars).Ra) > t1 && ((*vars).Rc - max((*vars).Rb,(*vars).Ra)) < t2)
    (*vars).Px = min((*vars).Ra, (*vars).Rb);
  else
    (*vars).Px = (*vars).Rb + (*vars).Ra - (*vars).Rc;
}
else if((*vars).Rc <= min((*vars).Ra,(*vars).Rb))
{
  if((min((*vars).Ra, (*vars).Rb) - (*vars).Rc) > t1 && abs((*vars).Rb - (*vars).Ra) < t2)
    (*vars).Px = (*vars).Ra + (*vars).Rd - (*vars).Rb;
  else
    (*vars).Px = (*vars).Rb + (*vars).Ra - (*vars).Rc;
  if(abs((*vars).Rb - (*vars).Ra) > t1 && ((min((*vars).Ra, (*vars).Rb) - (*vars).Rc) < t2)
    (*vars).Px = max((*vars).Ra, (*vars).Rb);
  else
    (*vars).Px = (*vars).Rb + (*vars).Ra - (*vars).Rc;
}
else
  (*vars).Px = (*vars).Rb + (*vars).Ra - (*vars).Rc;


///*********second*************///

if((*vars).Rc>=max((*vars).Ra,(*vars).Rb))
  {
    if((*vars).Ra <= (*vars).Rb)
      (*vars).Px = (*vars).Ra;
    else if(((*vars).Rc - max((*vars).Ra,(*vars).Rb)) > t1 && abs((*vars).Ra - (*vars).Rb) <= t2)
      //(*vars).Px = (a1 * (*vars).Rd + a2 * (*vars).Rb) / (a1 + a2);
      (*vars).Px = ((*vars).Ra + (*vars).Rb + (*vars).Rd) / 3;
      //(*vars).Px = (wa * (*vars).Ra + wb * (*vars).Rb + wc * (*vars).Rc + wd * (*vars).Rd) / (wa + wb + wc + wd);
    else
      (*vars).Px = (*vars).Rb;
  }
else if((*vars).Rc<=min((*vars).Ra,(*vars).Rb))
  {
    if((*vars).Ra >= (*vars).Rb)
      (*vars).Px = (*vars).Ra;
    //else if(((*vars).Rd - (*vars).Rb) >= t1 && ((*vars).Ra - (*vars).Rc) >= t2)
    else if((min((*vars).Ra,(*vars).Rb) - (*vars).Rc) > t1 && abs((*vars).Ra - (*vars).Rb) <= t2)
      //(*vars).Px = (a1 * (*vars).Rd + a2 * (*vars).Rb) / (a1 + a2);
      (*vars).Px = ((*vars).Ra + (*vars).Rb + (*vars).Rd) / 3;
      //(*vars).Px = (wa * (*vars).Ra + wb * (*vars).Rb + wc * (*vars).Rc + wd * (*vars).Rd) / (wa + wb + wc + wd);
    else
      (*vars).Px = (*vars).Rb;
  }
  else
    (*vars).Px = (*vars).Ra + (*vars).Rb - (*vars).Rc;
