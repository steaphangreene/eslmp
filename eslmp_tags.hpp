// *************************************************************************
//  This file is part of the esmp test project by Steaphan Greene
//
//  Copyright 2018 Steaphan Greene <steaphan@gmail.com>
//
//  esmp is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  esmp is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with esmp (see the file named "LICENSE");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

constexpr uint32_t tokenize(const uint8_t* const tag) {
  uint32_t token = tag[0];
  if (tag[0] == 'Z' && tag[1] == 'I' && tag[2] == 'A' && tag[3] == 'D') {
    token = '@';
  } else if (tag[0] == 'z' && tag[1] == 'I' && tag[2] == 'A' && tag[3] == 'D') {
    token = 0;
  } else if (tag[0] >= 'a' && tag[1] == '0' && tag[2] == 'T' && tag[3] == 'X') {
    token = tag[0] + '0' + 10 - 'a';
  } else if (tag[0] >= 'a') {
    token = tag[0] + 1 - 'a';
  }
  for (int ctr = 1; ctr < 4; ++ctr) {
    token <<= 8;
    token |= tag[ctr];
  }
  return token;
}
constexpr uint32_t tokenize(const char* const tag_str) {
  const uint8_t tag[4] = {
      (const uint8_t)tag_str[0],
      (const uint8_t)tag_str[1],
      (const uint8_t)tag_str[2],
      (const uint8_t)tag_str[3],
  };
  return tokenize(tag);
}

struct tag_type {
  bool is_container = false;
  bool is_big_item = false;
  bool is_metadata = false;
  bool is_string = false;
  bool is_known = false;
};

constexpr tag_type get_tag_props(uint32_t token) {
  tag_type ret = {false, false, false, false, false};
  switch (token) {
    default: {
      ret.is_container = false;
      ret.is_big_item = false;
      ret.is_metadata = false;
      ret.is_string = false;
      ret.is_known = false;
    } break;

    case (tokenize("ACEC")):
    case (tokenize("ACEP")):
    case (tokenize("ACID")):
    case (tokenize("ACPR")):
    case (tokenize("ACSR")):
    case (tokenize("ACUN")):
    case (tokenize("ATKR")):
    case (tokenize("NVSI")):
    case (tokenize("RCPR")):
    case (tokenize("SPMV")):
    case (tokenize("XATR")):
    case (tokenize("XWEM")):
    case (tokenize("RDGS")):
    case (tokenize("AVSK")):
    case (tokenize("BPND")):
    case (tokenize("BPNI")):
    case (tokenize("BPNN")):
    case (tokenize("BPNT")):
    case (tokenize("BPTN")):
    case (tokenize("DEMO")):
    case (tokenize("DEVA")):
    case (tokenize("DMAX")):
    case (tokenize("DMIN")):
    case (tokenize("EPF2")):
    case (tokenize("EPF3")):
    case (tokenize("EPFD")):
    case (tokenize("EPFT")):
    case (tokenize("FCHT")):
    case (tokenize("FPRT")):
    case (tokenize("HTID")):
    case (tokenize("ITXT")):
    case (tokenize("LCEC")):
    case (tokenize("LCEP")):
    case (tokenize("LCID")):
    case (tokenize("LCPR")):
    case (tokenize("LCSR")):
    case (tokenize("LCUN")):
    case (tokenize("MCHT")):
    case (tokenize("MO3S")):
    case (tokenize("MPRT")):
    case (tokenize("PRKC")):
    case (tokenize("PRKE")):
    case (tokenize("PRKF")):
    case (tokenize("SCRO")):
    case (tokenize("ALST")):
    case (tokenize("ALID")):
    case (tokenize("NAM0")):
    case (tokenize("NAM1")):
    case (tokenize("NAM2")):
    case (tokenize("NAM3")):
    case (tokenize("NAM4")):
    case (tokenize("NAM5")):
    case (tokenize("NAM6")):
    case (tokenize("NAM7")):
    case (tokenize("NAM8")):
    case (tokenize("NAM9")):
    case (tokenize("MO0T")):
    case (tokenize("MO1T")):
    case (tokenize("MO2T")):
    case (tokenize("MO3T")):
    case (tokenize("MO4T")):
    case (tokenize("MO5T")):
    case (tokenize("MO6T")):
    case (tokenize("MO7T")):
    case (tokenize("MO8T")):
    case (tokenize("MO9T")):
    case (tokenize("MODL")):
    case (tokenize("MODT")):
    case (tokenize("LNAM")):
    case (tokenize("DNAM")):
    case (tokenize("RNAM")):
    case (tokenize("OBND")):
    case (tokenize("BOD2")):
    case (tokenize("INTV")):
    case (tokenize("INCC")):
    case (tokenize("DATA")):
    case (tokenize("CNAM")):
    case (tokenize("HNAM")):
    case (tokenize("PNAM")):
    case (tokenize("WNAM")):
    case (tokenize("TNAM")):
    case (tokenize("DODT")):
    case (tokenize("FNAM")):
    case (tokenize("FLTV")):
    case (tokenize("FULL")):
    case (tokenize("DESC")):
    case (tokenize("VENV")):
    case (tokenize("PLVD")):
    case (tokenize("ICON")):
    case (tokenize("XNAM")):
    case (tokenize("CRVA")):
    case (tokenize("WAIT")):
    case (tokenize("STOL")):
    case (tokenize("PLCN")):
    case (tokenize("CRGR")):
    case (tokenize("JOUT")):
    case (tokenize("VEND")):
    case (tokenize("VENC")):
    case (tokenize("JAIL")):
    case (tokenize("MNAM")):
    case (tokenize("CITC")):
    case (tokenize("CTDA")):
    case (tokenize("SNAM")):
    case (tokenize("AHCF")):
    case (tokenize("AHCM")):
    case (tokenize("ANAM")):
    case (tokenize("ATKD")):
    case (tokenize("ATKE")):
    case (tokenize("BAMT")):
    case (tokenize("BIDS")):
    case (tokenize("BNAM")):
    case (tokenize("BODT")):
    case (tokenize("CNTO")):
    case (tokenize("COCT")):
    case (tokenize("COED")):
    case (tokenize("CRDT")):
    case (tokenize("DEST")):
    case (tokenize("DFTF")):
    case (tokenize("DFTM")):
    case (tokenize("DMDL")):
    case (tokenize("DMDS")):
    case (tokenize("DMDT")):
    case (tokenize("DSTD")):
    case (tokenize("DSTF")):
    case (tokenize("EAMT")):
    case (tokenize("EFID")):
    case (tokenize("EFIT")):
    case (tokenize("EITM")):
    case (tokenize("ENAM")):
    case (tokenize("ENIT")):
    case (tokenize("ETYP")):
    case (tokenize("FLMV")):
    case (tokenize("FNMK")):
    case (tokenize("FNPR")):
    case (tokenize("FTSF")):
    case (tokenize("FTSM")):
    case (tokenize("GNAM")):
    case (tokenize("HCLF")):
    case (tokenize("HEAD")):
    case (tokenize("INAM")):
    case (tokenize("INDX")):
    case (tokenize("KNAM")):
    case (tokenize("KSIZ")):
    case (tokenize("KWDA")):
    case (tokenize("MDOB")):
    case (tokenize("MO2S")):
    case (tokenize("MO4S")):
    case (tokenize("MODS")):
    case (tokenize("MPAI")):
    case (tokenize("MPAV")):
    case (tokenize("MTNM")):
    case (tokenize("MTYP")):
    case (tokenize("NAME")):
    case (tokenize("NNAM")):
    case (tokenize("ONAM")):
    case (tokenize("PFIG")):
    case (tokenize("PFPC")):
    case (tokenize("PHTN")):
    case (tokenize("PHWT")):
    case (tokenize("QNAM")):
    case (tokenize("QUAL")):
    case (tokenize("RDAT")):
    case (tokenize("RNMV")):
    case (tokenize("RPRF")):
    case (tokenize("RPRM")):
    case (tokenize("SDSC")):
    case (tokenize("SNDD")):
    case (tokenize("SNMV")):
    case (tokenize("SPCT")):
    case (tokenize("SPED")):
    case (tokenize("SPIT")):
    case (tokenize("SPLO")):
    case (tokenize("SWMV")):
    case (tokenize("TINC")):
    case (tokenize("TIND")):
    case (tokenize("TINI")):
    case (tokenize("TINL")):
    case (tokenize("TINP")):
    case (tokenize("TINT")):
    case (tokenize("TINV")):
    case (tokenize("TIRS")):
    case (tokenize("UNAM")):
    case (tokenize("UNES")):
    case (tokenize("VMAD")):
    case (tokenize("VNAM")):
    case (tokenize("VTCK")):
    case (tokenize("WBDT")):
    case (tokenize("WKMV")):
    case (tokenize("XMRK")):
    case (tokenize("YNAM")):
    case (tokenize("ZNAM")):
    case (tokenize("LVLG")):
    case (tokenize("LVLO")):
    case (tokenize("LVLD")):
    case (tokenize("LVLF")):
    case (tokenize("LLCT")):
    case (tokenize("IDLA")):
    case (tokenize("IDLC")):
    case (tokenize("IDLF")):
    case (tokenize("IDLT")):
    case (tokenize("SOUL")):
    case (tokenize("SLCP")):
    case (tokenize("NVMI")):
    case (tokenize("DALC")):
    case (tokenize("IMSP")):
    case (tokenize("JNAM")):
    case (tokenize("NVER")):
    case (tokenize("NVPP")):
    case (tokenize("RCLR")):
    case (tokenize("RDMO")):
    case (tokenize("RDMP")):
    case (tokenize("RDOT")):
    case (tokenize("RDSA")):
    case (tokenize("RDWT")):
    case (tokenize("RPLD")):
    case (tokenize("RPLI")):
    case (tokenize("WLST")):
    case (tokenize("MHDT")):
    case (tokenize("NAMA")):
    case (tokenize("OFST")):
    case (tokenize("PDTO")):
    case (tokenize("SCHR")):
    case (tokenize("SCTX")):
    case (tokenize("XACT")):
    case (tokenize("XALP")):
    case (tokenize("XAPD")):
    case (tokenize("XAPR")):
    case (tokenize("XCNT")):
    case (tokenize("XCVL")):
    case (tokenize("XCZA")):
    case (tokenize("XCZC")):
    case (tokenize("XEMI")):
    case (tokenize("XESP")):
    case (tokenize("XEZN")):
    case (tokenize("XFVC")):
    case (tokenize("XHOR")):
    case (tokenize("XHTW")):
    case (tokenize("XIS2")):
    case (tokenize("XLCM")):
    case (tokenize("XLCN")):
    case (tokenize("XLIB")):
    case (tokenize("XLIG")):
    case (tokenize("XLKR")):
    case (tokenize("XLOC")):
    case (tokenize("XLRM")):
    case (tokenize("XLRT")):
    case (tokenize("XLTW")):
    case (tokenize("XMBO")):
    case (tokenize("XMBP")):
    case (tokenize("XMBR")):
    case (tokenize("XNDP")):
    case (tokenize("XOCP")):
    case (tokenize("XOWN")):
    case (tokenize("XPOD")):
    case (tokenize("XPPA")):
    case (tokenize("XPRD")):
    case (tokenize("XPRM")):
    case (tokenize("XPWR")):
    case (tokenize("XRDS")):
    case (tokenize("XRGB")):
    case (tokenize("XRGD")):
    case (tokenize("XRMR")):
    case (tokenize("XRNK")):
    case (tokenize("XSCL")):
    case (tokenize("XSPC")):
    case (tokenize("XTEL")):
    case (tokenize("XTNM")):
    case (tokenize("XTRI")):
    case (tokenize("XWCN")):
    case (tokenize("XWCU")):
    case (tokenize("FLTR")):
    case (tokenize("LTMP")):
    case (tokenize("NEXT")):
    case (tokenize("QSDT")):
    case (tokenize("TCLT")):
    case (tokenize("TIFC")):
    case (tokenize("TRDT")):
    case (tokenize("TWAT")):
    case (tokenize("WCTR")):
    case (tokenize("ALCA")):
    case (tokenize("ALCL")):
    case (tokenize("ALCO")):
    case (tokenize("ALDN")):
    case (tokenize("ALEA")):
    case (tokenize("ALED")):
    case (tokenize("ALEQ")):
    case (tokenize("ALFA")):
    case (tokenize("ALFC")):
    case (tokenize("ALFD")):
    case (tokenize("ALFE")):
    case (tokenize("ALFI")):
    case (tokenize("ALFL")):
    case (tokenize("ALFR")):
    case (tokenize("ALLS")):
    case (tokenize("ALNA")):
    case (tokenize("ALNT")):
    case (tokenize("ALPC")):
    case (tokenize("ALRT")):
    case (tokenize("ALSP")):
    case (tokenize("ALUA")):
    case (tokenize("CIS1")):
    case (tokenize("CSCR")):
    case (tokenize("CSFL")):
    case (tokenize("CSGD")):
    case (tokenize("CSLR")):
    case (tokenize("CSMD")):
    case (tokenize("CSME")):
    case (tokenize("ECOR")):
    case (tokenize("ICO2")):
    case (tokenize("PFO2")):
    case (tokenize("PFOR")):
    case (tokenize("PKC2")):
    case (tokenize("PKCU")):
    case (tokenize("PKDT")):
    case (tokenize("PLDT")):
    case (tokenize("POBA")):
    case (tokenize("POCA")):
    case (tokenize("POEA")):
    case (tokenize("PRCB")):
    case (tokenize("PSDT")):
    case (tokenize("PTDA")):
    case (tokenize("QOBJ")):
    case (tokenize("QSTA")):
    case (tokenize("QTGL")):
    case (tokenize("SCDA")):
    case (tokenize("SPOR")):
    case (tokenize("TPIC")):
    case (tokenize("zIAD")):
    case (tokenize("aIAD")):
    case (tokenize("bIAD")):
    case (tokenize("cIAD")):
    case (tokenize("dIAD")):
    case (tokenize("eIAD")):
    case (tokenize("fIAD")):
    case (tokenize("gIAD")):
    case (tokenize("hIAD")):
    case (tokenize("iIAD")):
    case (tokenize("jIAD")):
    case (tokenize("kIAD")):
    case (tokenize("lIAD")):
    case (tokenize("mIAD")):
    case (tokenize("nIAD")):
    case (tokenize("oIAD")):
    case (tokenize("pIAD")):
    case (tokenize("qIAD")):
    case (tokenize("rIAD")):
    case (tokenize("sIAD")):
    case (tokenize("tIAD")):
    case (tokenize("@IAD")):
    case (tokenize("AIAD")):
    case (tokenize("BIAD")):
    case (tokenize("CIAD")):
    case (tokenize("DIAD")):
    case (tokenize("EIAD")):
    case (tokenize("FIAD")):
    case (tokenize("GIAD")):
    case (tokenize("HIAD")):
    case (tokenize("IIAD")):
    case (tokenize("JIAD")):
    case (tokenize("KIAD")):
    case (tokenize("LIAD")):
    case (tokenize("MIAD")):
    case (tokenize("NIAD")):
    case (tokenize("OIAD")):
    case (tokenize("PIAD")):
    case (tokenize("QIAD")):
    case (tokenize("RIAD")):
    case (tokenize("SIAD")):
    case (tokenize("TIAD")): {
      ret.is_container = false;
      ret.is_big_item = false;
      ret.is_metadata = false;
      ret.is_string = false;
      ret.is_known = true;
    } break;

    case (tokenize("HEDR")):
    case (tokenize("XXXX")): {
      ret.is_container = false;
      ret.is_big_item = false;
      ret.is_metadata = true;
      ret.is_string = false;
      ret.is_known = true;
    } break;

    case (tokenize("00TX")):
    case (tokenize("10TX")):
    case (tokenize("20TX")):
    case (tokenize("30TX")):
    case (tokenize("40TX")):
    case (tokenize("50TX")):
    case (tokenize("60TX")):
    case (tokenize("70TX")):
    case (tokenize("80TX")):
    case (tokenize("90TX")):
    case (tokenize("a0TX")):
    case (tokenize("b0TX")):
    case (tokenize("c0TX")):
    case (tokenize("d0TX")):
    case (tokenize("e0TX")):
    case (tokenize("f0TX")):
    case (tokenize("g0TX")):
    case (tokenize("A0TX")):
    case (tokenize("B0TX")):
    case (tokenize("C0TX")):
    case (tokenize("D0TX")):
    case (tokenize("E0TX")):
    case (tokenize("F0TX")):
    case (tokenize("G0TX")):
    case (tokenize("H0TX")):
    case (tokenize("I0TX")):
    case (tokenize("J0TX")):
    case (tokenize("K0TX")):
    case (tokenize("L0TX")):
    case (tokenize("M0TX")):
    case (tokenize("N0TX")):
    case (tokenize("O0TX")):
    case (tokenize("P0TX")):
    case (tokenize("Q0TX")):
    case (tokenize("CIS2")):
    case (tokenize("MAST")):
    case (tokenize("EDID")):
    case (tokenize("TX00")):
    case (tokenize("TX01")):
    case (tokenize("TX02")):
    case (tokenize("TX03")):
    case (tokenize("TX04")):
    case (tokenize("TX05")):
    case (tokenize("TX06")):
    case (tokenize("TX07")):
    case (tokenize("TX08")):
    case (tokenize("TX09")):
    case (tokenize("MOD0")):
    case (tokenize("MOD1")):
    case (tokenize("MOD2")):
    case (tokenize("MOD3")):
    case (tokenize("MOD4")):
    case (tokenize("MOD5")):
    case (tokenize("MOD6")):
    case (tokenize("MOD7")):
    case (tokenize("MOD8")):
    case (tokenize("MOD9")): {
      ret.is_container = false;
      ret.is_big_item = false;
      ret.is_metadata = false;
      ret.is_string = true;
      ret.is_known = true;
    } break;

    case (tokenize("NPC_")):
    case (tokenize("CELL")):
    case (tokenize("NAVM")):
    case (tokenize("LAND")): {
      ret.is_container = true;
      ret.is_big_item = true;
      ret.is_metadata = false;
      ret.is_string = false;
      ret.is_known = true;
    } break;

    case (tokenize("TES4")):
    case (tokenize("GRUP")):
    case (tokenize("GMST")):
    case (tokenize("KYWD")):
    case (tokenize("LCRT")):
    case (tokenize("AACT")):
    case (tokenize("TXST")):
    case (tokenize("GLOB")):
    case (tokenize("CLAS")):
    case (tokenize("FACT")):
    case (tokenize("HDPT")):
    case (tokenize("HAIR")):
    case (tokenize("EYES")):
    case (tokenize("RACE")):
    case (tokenize("SOUN")):
    case (tokenize("ASPC")):
    case (tokenize("MGEF")):
    case (tokenize("SCPT")):
    case (tokenize("LTEX")):
    case (tokenize("ENCH")):
    case (tokenize("SPEL")):
    case (tokenize("SCRL")):
    case (tokenize("ACTI")):
    case (tokenize("TACT")):
    case (tokenize("ARMO")):
    case (tokenize("BOOK")):
    case (tokenize("CONT")):
    case (tokenize("DOOR")):
    case (tokenize("INGR")):
    case (tokenize("LIGH")):
    case (tokenize("MISC")):
    case (tokenize("APPA")):
    case (tokenize("STAT")):
    case (tokenize("SCOL")):
    case (tokenize("MSTT")):
    case (tokenize("PWAT")):
    case (tokenize("GRAS")):
    case (tokenize("TREE")):
    case (tokenize("CLDC")):
    case (tokenize("FLOR")):
    case (tokenize("FURN")):
    case (tokenize("WEAP")):
    case (tokenize("AMMO")):
    case (tokenize("LVLN")):
    case (tokenize("KEYM")):
    case (tokenize("ALCH")):
    case (tokenize("IDLM")):
    case (tokenize("COBJ")):
    case (tokenize("PROJ")):
    case (tokenize("HAZD")):
    case (tokenize("SLGM")):
    case (tokenize("LVLI")):
    case (tokenize("WTHR")):
    case (tokenize("CLMT")):
    case (tokenize("SPGD")):
    case (tokenize("RFCT")):
    case (tokenize("REGN")):
    case (tokenize("NAVI")):
    case (tokenize("WRLD")):
    case (tokenize("DIAL")):
    case (tokenize("QUST")):
    case (tokenize("IDLE")):
    case (tokenize("PACK")):
    case (tokenize("CSTY")):
    case (tokenize("LSCR")):
    case (tokenize("LVSP")):
    case (tokenize("ANIO")):
    case (tokenize("WATR")):
    case (tokenize("EFSH")):
    case (tokenize("EXPL")):
    case (tokenize("DEBR")):
    case (tokenize("IMGS")):
    case (tokenize("IMAD")):
    case (tokenize("FLST")):
    case (tokenize("PERK")):
    case (tokenize("BPTD")):
    case (tokenize("ADDN")):
    case (tokenize("AVIF")):
    case (tokenize("CAMS")):
    case (tokenize("CPTH")):
    case (tokenize("VTYP")):
    case (tokenize("MATT")):
    case (tokenize("IPCT")):
    case (tokenize("IPDS")):
    case (tokenize("ARMA")):
    case (tokenize("ECZN")):
    case (tokenize("LCTN")):
    case (tokenize("MESG")):
    case (tokenize("RGDL")):
    case (tokenize("DOBJ")):
    case (tokenize("LGTM")):
    case (tokenize("MUSC")):
    case (tokenize("FSTP")):
    case (tokenize("FSTS")):
    case (tokenize("SMBN")):
    case (tokenize("SMQN")):
    case (tokenize("SMEN")):
    case (tokenize("DLBR")):
    case (tokenize("MUST")):
    case (tokenize("DLVW")):
    case (tokenize("WOOP")):
    case (tokenize("SHOU")):
    case (tokenize("EQUP")):
    case (tokenize("RELA")):
    case (tokenize("SCEN")):
    case (tokenize("ASTP")):
    case (tokenize("OTFT")):
    case (tokenize("ARTO")):
    case (tokenize("MATO")):
    case (tokenize("MOVT")):
    case (tokenize("SNDR")):
    case (tokenize("DUAL")):
    case (tokenize("SNCT")):
    case (tokenize("SOPM")):
    case (tokenize("COLL")):
    case (tokenize("CLFM")):
    case (tokenize("REVB")):
    case (tokenize("REFR")):
    case (tokenize("ACHR")):
    case (tokenize("PGRE")):
    case (tokenize("PHZD")):
    case (tokenize("VOLI")):
    case (tokenize("INFO")): {
      ret.is_container = true;
      ret.is_big_item = false;
      ret.is_metadata = false;
      ret.is_string = false;
      ret.is_known = true;
    } break;
  }
  return ret;
}
