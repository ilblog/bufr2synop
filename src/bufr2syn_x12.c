/***************************************************************************
 *   Copyright (C) 2012 by Guillermo Ballester Valor                       *
 *   gbv@ogimet.com                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*!
 \file bufr2syn_x12.c
 \brief decodes the descriptors with X = 12
 */
#include "bufr2synop.h"

char * kelvin_to_snTTT ( char *target, double T )
{
  int ic;
  if ( T < 150.0 || T > 340.0)
    return NULL;
  ic = ( int ) (100.0 * T + 0.001) - 27315;
  ic /= 10;
  if ( ic < 0 )
    sprintf ( target, "1%03d", -ic );
  else
    sprintf ( target, "%04d", ic );
  return target;
}

char * kelvin_to_snTT ( char *target, double T )
{
  int ic;
  if ( T < 150.0 || T > 340.0)
    return NULL;
  ic = ( int ) (100.0 * T + 0.001) - 27315;
  ic /= 100;
  if ( ic < 0 )
    sprintf ( target, "1%02d", -ic );
  else
    sprintf ( target, "%03d", ic );
  return target;
}

int syn_parse_x12 ( struct synop_chunks *syn, struct bufr_subset_state *s, char *err )
{
  char aux[16];

  switch ( s->a->desc.y )
    {
    case 1: // 0 12 001
    case 4: // 0 12 004
    case 101: // 0 12 101
    case 104: // 0 12 104
      if (syn->s1.TTT[0] == 0)
        {
          kelvin_to_snTTT ( aux, s->val );
          syn->s1.sn1[0] = aux[0];
          strcpy ( syn->s1.TTT, aux + 1 );
        }
      break;
    case 3: // 0 12 003
    case 6: // 0 12 006
    case 103: // 0 12 103
    case 106: // 0 12 106
      if (syn->s1.TdTdTd[0] == 0)
        {
          kelvin_to_snTTT ( aux, s->val );
          syn->s1.sn2[0] = aux[0];
          strcpy ( syn->s1.TdTdTd, aux + 1 );
        }
      break;
    case 11: // 0 12 011
    case 14: // 0 12 014
    case 21: // 0 12 021
    case 111: // 0 12 115
    case 114: // 0 12 114
    case 116: // 0 12 116
      if (syn->s3.TxTxTx[0] == 0)
        {
          kelvin_to_snTTT ( aux, s->val );
          syn->s3.snx[0] = aux[0];
          strcpy ( syn->s3.TxTxTx, aux + 1 );
          syn->mask |= SYNOP_SEC3;
        }
      break;
    case 12: // 0 12 012
    case 15: // 0 12 015
    case 22: // 0 12 022
    case 112: // 0 12 115
    case 115: // 0 12 115
    case 117: // 0 12 117
      if (syn->s3.TnTnTn[0] == 0)
        {
          kelvin_to_snTTT ( aux, s->val );
          syn->s3.snn[0] = aux[0];
          strcpy ( syn->s3.TnTnTn, aux + 1 );
          syn->mask |= SYNOP_SEC3;
        }
      break;
    case 113:
      if (strcmp("6", guess_WMO_region(syn)) == 0) // region VI
        {
          kelvin_to_snTT ( aux, s->val );
          strcpy ( syn->s3.jjj, aux);
          syn->mask |= SYNOP_SEC3;
        }
      break;
    default:
      break;
    }
  return 0;
}
