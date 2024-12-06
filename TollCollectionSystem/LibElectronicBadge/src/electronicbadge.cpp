#include "electronicbadge.h"

#include "electronicbadgemgskj.h"

ElectronicBadge::ElectronicBadge() {}

ElectronicBadge* getElectronicBadge(int type) {
    ElectronicBadge* electronicBadge = nullptr;

    switch (type) {
        case MGSKJ_ELECTRONIC_BADGE:
            electronicBadge = new ElectronicBadgeMgskj();
            break;
        default:
            break;
    }

    return electronicBadge;
}
