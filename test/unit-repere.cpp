#include "catch.hpp"

#include "MathToolbox/Repere.h"

using namespace repere;

TEST_CASE("Repere") {
    Repere ref1({1_m, 2_m}, Multiplier::SENS_POSITIVE, Multiplier::SENS_POSITIVE);
    Repere ref2({-2_m, 2_m}, Multiplier::SENS_POSITIVE, Multiplier::SENS_NEGATIVE);
    Repere ref3({1_m, 1_m}, Multiplier::SENS_NEGATIVE, Multiplier::SENS_POSITIVE);
    Repere ref4({-2_m, -1_m}, Multiplier::SENS_NEGATIVE, Multiplier::SENS_NEGATIVE);

    SECTION("Repere equality") {
        CHECK(ref1 == ref1);
        CHECK_FALSE(ref1 == ref2);
        CHECK_FALSE(ref1 == ref3);
        CHECK_FALSE(ref1 == ref4);

        Repere almostRef1_1({1_m, Length::makeFromM(2 + std::numeric_limits<double>::epsilon() / 2)},
                            Multiplier::SENS_POSITIVE, Multiplier::SENS_POSITIVE);
        CHECK(ref1 == almostRef1_1);
    }

    SECTION("Copy operators") {
        Repere ref1Bis(ref1);
        CHECK(ref1 == ref1Bis);
        CHECK((ref1.getOrigine() - ref1Bis.getOrigine()).norm().toM() == Approx(0));
    }

    SECTION("Angle conversion") {
        Angle angle = 52_deg; // in ref1
        CHECK(ref1.getAngle(angle, ref1).toDeg() == Approx(52));
        CHECK(ref2.getAngle(angle, ref1).toDeg() == Approx(-52));
        CHECK(ref3.getAngle(angle, ref1).toDeg() == Approx(180 - 52));
        CHECK(ref4.getAngle(angle, ref1).toDeg() == Approx(52 - 180));
    }

    SECTION("Delta angle conversion") {
        Angle delta = 52_deg; // in ref1
        CHECK(ref1.convertDeltaAngle(ref1, delta).toDeg() == Approx(52));
        CHECK(ref2.convertDeltaAngle(ref1, delta).toDeg() == Approx(-52));
        CHECK(ref3.convertDeltaAngle(ref1, delta).toDeg() == Approx(-52));
        CHECK(ref4.convertDeltaAngle(ref1, delta).toDeg() == Approx(52));
    }

    SECTION("Position conversion") {
        Vector3m pos {6.7_m, 5.2_m, 1_m}; // in ref1
        Vector3m pos1 = ref1.getPosition(pos, ref1);
        CHECK(pos1.x.toM() == Approx(6.7));
        CHECK(pos1.y.toM() == Approx(5.2));
        Vector3m pos2 = ref2.getPosition(pos, ref1);
        CHECK(pos2.x.toM() == Approx(9.7));
        CHECK(pos2.y.toM() == Approx(-5.2));
        Vector3m pos3 = ref3.getPosition(pos, ref1);
        CHECK(pos3.x.toM() == Approx(-6.7));
        CHECK(pos3.y.toM() == Approx(6.2));
        Vector3m pos4 = ref4.getPosition(pos, ref1);
        CHECK(pos4.x.toM() == Approx(-9.7));
        CHECK(pos4.y.toM() == Approx(-8.2));
    }
}
