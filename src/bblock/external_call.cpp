/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "bblock/system.h"
#include <cstdlib>
/**
 * @file external_call.cpp
 * @brief Functions to use the system class and its energy functions from other languages
 */

namespace {
bblock::System* my_s;
std::vector<bblock::System*> my_sf;
}  // namespace

extern "C" {
/**
 * Initializes the system in the heap.
 * @param[in] coords Pointer to the coordinates (size 3N)
 * @param[in] nat_monomers Pointer to an array of the number of atoms in each monomer
 * @param[in] at_names Pointer to an array with the atom names of all the whole system
 * @param[in] monomers Pointer to the list of monomer ids in your system
 * @param[in] nmon Number of monomers
 * @param[in] json_file Name of the json configuration file
 */
void initialize_system_(double* coords, int* nat_monomers, char at_names[][5], char monomers[][5], int* nmon,
                        char json_file[20]) {
    my_s = new bblock::System();
    int count = 0;
    for (int i = 0; i < *nmon; i++) {
        std::vector<double> xyz(3 * nat_monomers[i]);
        std::vector<std::string> vAtNames(nat_monomers[i]);

        std::copy(coords + 3 * count, coords + 3 * (count + nat_monomers[i]), xyz.begin());
        std::copy(at_names + count, at_names + count + nat_monomers[i], vAtNames.begin());
        std::string id = monomers[i];
        my_s->AddMonomer(xyz, vAtNames, id);
        count += nat_monomers[i];
    }

    my_s->Initialize();
    my_s->SetUpFromJson(json_file);
}

void initialize_system2_(double* coords, int* nat_monomers, char at_names[][5], char monomers[][5], int* nmon,
                         char json_file[20], int* nranks) {
    for (size_t k = 0; k < *nranks; k++) {
        my_sf.push_back(new bblock::System());
        int count = 0;
        for (int i = 0; i < *nmon; i++) {
            std::vector<double> xyz(3 * nat_monomers[i]);
            std::vector<std::string> vAtNames(nat_monomers[i]);

            std::copy(coords + 3 * count, coords + 3 * (count + nat_monomers[i]), xyz.begin());
            std::copy(at_names + count, at_names + count + nat_monomers[i], vAtNames.begin());
            std::string id = monomers[i];
            my_sf[k]->AddMonomer(xyz, vAtNames, id);
            count += nat_monomers[i];
        }

        my_sf[k]->Initialize();
        my_sf[k]->SetUpFromJson(json_file);
    }
}

/**
 * Initializes the system in the heap.
 * @param[in] coords Pointer to the coordinates (size 3N)
 * @param[in] nat_monomers Pointer to an array of the number of atoms in each monomer
 * @param[in] at_names Pointer to an array with the atom names of all the whole system
 * @param[in] monomers Pointer to the list of monomer ids in your system
 * @param[in] nmon Number of monomers
 * @param[in] json_file Name of the json configuration file
 */
void initialize_system_py_(double* coords, int* nat_monomers, char** at_names, char** monomers, int* nmon,
                           char* json_file) {
    my_s = new bblock::System();
    int count = 0;
    for (int i = 0; i < *nmon; i++) {
        std::vector<double> xyz(3 * nat_monomers[i]);
        std::vector<std::string> vAtNames(nat_monomers[i]);

        std::copy(coords + 3 * count, coords + 3 * (count + nat_monomers[i]), xyz.begin());
        std::copy(at_names + count, at_names + count + nat_monomers[i], vAtNames.begin());
        std::string id = monomers[i];
        my_s->AddMonomer(xyz, vAtNames, id);
        count += nat_monomers[i];
    }

    my_s->Initialize();
    my_s->SetUpFromJson(json_file);
}

/**
 * Given the coordinates, calculates the energy for a gas phase system
 * @param[in] coords Pointer to the coordinates (size 3N)
 * @param[in] nat Number of atoms in he system
 * @param[out] energy Energy of the system
 */
void get_energy_(double* coords, int* nat, double* energy) {
    std::vector<double> xyz(3 * (*nat));
    std::copy(coords, coords + 3 * (*nat), xyz.begin());

    my_s->SetRealXyz(xyz);
    *energy = my_s->Energy(false);
}

void get_energy2_(double* coords, int* nat, double* energy, int* rank) {
    std::vector<double> xyz(3 * (*nat));
    std::copy(coords, coords + 3 * (*nat), xyz.begin());

    my_sf[*rank]->SetRealXyz(xyz);
    *energy = my_sf[*rank]->Energy(false);
}

/**
 * Given the coordinates, calculates the energy iand gradients for a gas phase system
 * @param[in] coords Pointer to the coordinates (size 3N)
 * @param[in] nat Number of atoms in he system
 * @param[out] energy Energy of the system
 * @param[out] grads Pointer to the gradients of the system (size 3N)
 */
void get_energy_g_(double* coords, int* nat, double* energy, double* grads) {
    std::vector<double> xyz(3 * (*nat));
    std::copy(coords, coords + 3 * (*nat), xyz.begin());

    my_s->SetRealXyz(xyz);
    *energy = my_s->Energy(true);

    std::vector<double> gradv = my_s->GetRealGrads();
    std::copy(gradv.begin(), gradv.end(), grads);
}

void get_energy_g2_(double* coords, int* nat, double* energy, double* grads, int* rank) {
    std::vector<double> xyz(3 * (*nat));
    std::copy(coords, coords + 3 * (*nat), xyz.begin());

    my_sf[*rank]->SetRealXyz(xyz);
    *energy = my_sf[*rank]->Energy(true);

    std::vector<double> gradv = my_sf[*rank]->GetRealGrads();
    std::copy(gradv.begin(), gradv.end(), grads);
}

/**
 * Given the coordinates, calculates the energy for PBC systems
 * @param[in] coords Pointer to the coordinates (size 3N)
 * @param[in] nat Number of atoms in he system
 * @param[in] box Pointer to the array with the box (size 9)
 * @param[out] energy Energy of the system
 */
void get_energy_pbc_(double* coords, int* nat, double* box, double* energy) {
    std::vector<double> xyz(3 * (*nat));
    std::vector<double> boxv(9, 0.0);
    std::copy(coords, coords + 3 * (*nat), xyz.begin());
    std::copy(box, box + 9, boxv.begin());

    my_s->SetRealXyz(xyz);
    my_s->SetPBC(boxv);
    *energy = my_s->Energy(false);
}

/**
 * Given the coordinates, calculates the energy iand gradients for a PBC system
 * @param[in] coords Pointer to the coordinates (size 3N)
 * @param[in] nat Number of atoms in he system
 * @param[in] box Pointer to the array with the box (size 9)
 * @param[out] energy Energy of the system
 * @param[out] grads Pointer to the gradients of the system (size 3N)
 */
void get_energy_pbc_g_(double* coords, int* nat, double* box, double* energy, double* grads) {
    std::vector<double> xyz(3 * (*nat));
    std::vector<double> boxv(9, 0.0);
    std::copy(coords, coords + 3 * (*nat), xyz.begin());
    std::copy(box, box + 9, boxv.begin());

    my_s->SetRealXyz(xyz);
    my_s->SetPBC(boxv);
    *energy = my_s->Energy(true);

    std::vector<double> gradv = my_s->GetRealGrads();
    std::copy(gradv.begin(), gradv.end(), grads);
}

void get_total_dipole_(double* dip) {
    std::vector<double> dtot(3, 0.0), dperm(3, 0.0), dind(3, 0.0);
    my_s->GetTotalDipole(dperm, dind, dtot);
    std::copy(dtot.begin(), dtot.end(), dip);
}

/**
 * Retrieves the virial from the system class
 * @param[out] virial Pointer to the virial. Must be a 9 element vector.
 */

void get_virial_(double* virial) {
    std::vector<double> v = my_s->GetVirial();
    if (v.size() < 1) {
        std::fill(virial, virial + 9, 0.0);
    } else if (v.size() == 9) {
        std::copy(v.begin(), v.begin() + 9, virial);
    } else {
        std::cerr << "Cannot retrieve the virial. Size should be 9, and instead is " << v.size() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void set_real_xyz_(double* coords, int* nat) {
    std::vector<double> xyz(coords, coords + 3 * (*nat));
    my_s->SetRealXyz(xyz);
}

void get_xyz_(double* coords) {
    std::vector<double> xyz = my_s->GetXyz();
    std::copy(xyz.begin(), xyz.end(), coords);
}

void get_potential_and_electric_field_on_points_(double* coords, double* phi, double* ef, int* nat) {
    std::vector<double> xyz(coords, coords + 3 * (*nat));
    my_s->Hack3GetPotentialAtPoints(xyz);
    std::vector<double> p, e, pd, ed;
    my_s->GetPhiXAndEfX(p, e, pd, ed);
    std::copy(p.begin(), p.end(), phi);
    std::copy(e.begin(), e.end(), ef);
    for (size_t i = 0; i < p.size(); i++) phi[i] += pd[i];
    for (size_t i = 0; i < e.size(); i++) ef[i] += ed[i];
}

void set_box_(int* length, double* box) {
    if (*length > 0) {
        std::vector<double> boxv(box, box + (*length));
        my_s->SetPBC(boxv);
    } else {
        std::vector<double> boxv;
        my_s->SetPBC(boxv);
    }
    my_s->SetNewParamsElec(false);
}

void set_potential_and_electric_field_on_sites_(double* phi, double* ef, int* nsites) {
    std::vector<double> phiv(phi, phi + (*nsites));
    std::vector<double> efv(ef, ef + 3 * (*nsites));
    my_s->SetExternalElectrostaticPotentialAndFieldInSites(phiv, efv);
}

void get_charges_(double* charges) {
    std::vector<double> chg = my_s->GetCharges();
    std::copy(chg.begin(), chg.end(), charges);
}

void get_induced_dipoles_(double* mu_i_out) {
    std::vector<double> mu_p, mu_i;
    my_s->GetDipoles(mu_p, mu_i);
    std::copy(mu_i.begin(), mu_i.end(), mu_i_out);
}

/**
 * VK 
 */
void get_permanent_dipoles_(double* mu_p_out) {
    std::vector<double> mu_p, mu_i;
    my_s->GetDipoles(mu_p, mu_i);
    std::copy(mu_p.begin(), mu_p.end(), mu_p_out);
}

void redistribute_gradients_(double* grd, int* nsites) {
    std::vector<double> grad(grd, grd + 3 * (*nsites));
    my_s->RedistributeGradients(grad);
    std::copy(grad.begin(), grad.end(), grd);
}

void get_polarizabilities_(double* polarizability) {
    std::vector<double> pol = my_s->GetPolarizabilities();
    std::copy(pol.begin(), pol.end(), polarizability);
}

void get_external_field_contribution_to_energy_(double* e) {
    *e = my_s->GetPermanentElectrostaticEnergyExternalFieldContribution();
}

/**
 * Deletes the pointer to the system
 */
void finalize_system_() { delete my_s; }
void finalize_system2_() {
    for (size_t k = 0; k < my_sf.size(); k++) delete my_sf[k];
}

}  // extern C
