#include "Material.h"

void Material::bind(const Program &program) const {
    program.set3fv("material.Ka", Ka);
    program.set3fv("material.Kd", Kd);
    program.set3fv("material.Ks", Ks);
    program.set3fv("material.Ke", Ke);

    program.setFloat("material.Ns", Ns);
    program.setFloat("material.d", d);

    uint32_t flags = 0;
    if (map_Ka > -1) {
        program.bindTexture("ambientTexture", map_Ka, 0);
        flags |= 1 << USE_AMBIENT_TEXTURE;
    }
    if (map_Kd > -1) {
        program.bindTexture("diffuseTexture", map_Kd, 1);
        flags |= 1 << USE_DIFFUSE_TEXTURE;
    }
    if (map_Ks > -1) {
        program.bindTexture("specularTexture", map_Ks, 2);
        flags |= 1 << USE_SPECULAR_TEXTURE;
    }
    if (map_Ke > -1) {
        program.bindTexture("emissveTexture", map_Ke, 3);
        flags |= 1 << USE_EMISSIVE_TEXTURE;
    }
    if (map_Ns > -1) {
        program.bindTexture("specularHighlightTexture", map_Ns, 4);
        flags |= 1 << USE_SPECULAR_HIGHLIGHT_TEXTURE;
    }
    if (map_bump > -1) {
        program.bindTexture("bumpTexture", map_bump, 5);
        flags |= 1 << USE_BUMP_TEXTURE;
    }
    program.setUInt("texFlags", flags);
}

