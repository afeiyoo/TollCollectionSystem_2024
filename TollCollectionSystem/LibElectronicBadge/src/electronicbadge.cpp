#include "electronicbadge.h"

#include "electronicbadgemgskj.h"

ElectronicBadge::ElectronicBadge() {}

ElectronicBadge::~ElectronicBadge() {
    // 需要定义纯虚函数
    // 当析构父类指针时，可以调用子类对象的析构函数
}

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
