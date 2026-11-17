#include "scale_tests.h"

#include "../../src/bonus/bonus_scale.h"

enum {
    TWICE = 2,
    THRICE = 3,
    SET = 0,
    RESOURCE = 1,
    ADD = 4,
    MULTIPLY = 5,
    MULTIPLY_RESOURCE = 6,
    SPAWN = 7,
    TECH_COST = 101,
    TECH_TIME = 103,
    NONE = -1,
    FOOD = 0,
    HIT_POINTS = 0,
    SPEED = 5,
    ARMOR = 8,
    ATTACK = 9,
    INFANTRY = 6,
    MODE_ADD = 1,
    MODE_MULTIPLY = 2,
    LOOM = 22,
    MILITIA = 74,
    BARRACKS = 12,
    EFFECT_FUNCTION = 33,
    SPAWN_LIMIT = 234,
    PIERCE = 768
};

static const float CLOSE = 0.001F;

static bool near(float value, float expected)
{
    return (value > expected - CLOSE) && (value < expected + CLOSE);
}

static float first_amount(DatChange change, int multiplier)
{
    return bonus_scale(&change, multiplier).changes[0].d;
}

static float second_amount(DatChange change, int multiplier)
{
    ScaledChanges scaled = bonus_scale(&change, multiplier);

    return (scaled.count == TWICE) ? scaled.changes[1].d : 0.0F;
}

bool test_an_added_amount_taken_three_times_is_tripled(void)
{
    return near(first_amount((DatChange){ ADD, NONE, INFANTRY, HIT_POINTS, 15.0F }, THRICE), 45.0F);
}

bool test_a_multiplied_attribute_taken_twice_is_squared(void)
{
    return near(first_amount((DatChange){ MULTIPLY, NONE, INFANTRY, SPEED, 1.5F }, TWICE), 2.25F);
}

bool test_a_multiplied_resource_taken_twice_is_squared(void)
{
    return near(first_amount((DatChange){ MULTIPLY_RESOURCE, FOOD, NONE, NONE, 1.5F }, TWICE), 2.25F);
}

bool test_a_tech_cost_multiplied_in_mode_two_taken_twice_is_squared(void)
{
    return near(first_amount((DatChange){ TECH_COST, LOOM, FOOD, MODE_MULTIPLY, 0.5F }, TWICE), 0.25F);
}

bool test_a_tech_time_multiplied_in_mode_two_taken_twice_is_squared(void)
{
    return near(first_amount((DatChange){ TECH_TIME, LOOM, NONE, MODE_MULTIPLY, 0.5F }, TWICE), 0.25F);
}

bool test_a_tech_cost_added_in_mode_one_is_kept(void)
{
    return near(first_amount((DatChange){ TECH_COST, LOOM, FOOD, MODE_ADD, -10.0F }, TWICE), -10.0F);
}

bool test_a_spawn_taken_twice_makes_twice_the_units(void)
{
    DatChange spawn = { SPAWN, MILITIA, BARRACKS, 3, 0.0F };

    return bonus_scale(&spawn, TWICE).changes[0].c == 6;
}

bool test_a_resource_taken_twice_is_doubled(void)
{
    return near(first_amount((DatChange){ RESOURCE, FOOD, MODE_ADD, NONE, 100.0F }, TWICE), 200.0F);
}

bool test_the_spawn_limit_is_kept(void)
{
    return near(first_amount((DatChange){ RESOURCE, SPAWN_LIMIT, MODE_ADD, NONE, 5.0F }, TWICE), 5.0F);
}

bool test_the_effect_function_is_kept(void)
{
    return near(first_amount((DatChange){ RESOURCE, EFFECT_FUNCTION, SET, NONE, 7.0F }, TWICE), 7.0F);
}

bool test_a_set_attribute_is_kept(void)
{
    return near(first_amount((DatChange){ SET, NONE, INFANTRY, HIT_POINTS, 100.0F }, TWICE), 100.0F);
}

bool test_an_armor_added_past_255_is_split_in_two(void)
{
    return near(second_amount((DatChange){ ADD, NONE, INFANTRY, ARMOR, (float)(PIERCE + 200) }, TWICE),
                (float)(PIERCE + 145));
}

bool test_an_armor_taken_away_keeps_its_sign(void)
{
    return near(first_amount((DatChange){ ADD, NONE, INFANTRY, ARMOR, (float)-(PIERCE + 2) }, TWICE),
                (float)-(PIERCE + 4));
}

bool test_an_attack_multiplied_past_255_percent_is_split_in_two(void)
{
    return near(second_amount((DatChange){ MULTIPLY, NONE, INFANTRY, ATTACK, (float)(PIERCE + 150) }, THRICE),
                (float)(PIERCE + 132));
}

bool test_a_multiplied_attack_halfway_is_rounded_to_even(void)
{
    return near(first_amount((DatChange){ MULTIPLY, NONE, INFANTRY, ATTACK, (float)(PIERCE + 50) }, THRICE),
                (float)(PIERCE + 12));
}

bool test_a_multiplied_attack_past_a_half_is_rounded_up(void)
{
    return near(first_amount((DatChange){ MULTIPLY, NONE, INFANTRY, ATTACK, (float)(PIERCE + 109) }, TWICE),
                (float)(PIERCE + 119));
}

bool test_a_multiplied_attack_below_a_half_is_rounded_down(void)
{
    return near(first_amount((DatChange){ MULTIPLY, NONE, INFANTRY, ATTACK, (float)(PIERCE + 111) }, TWICE),
                (float)(PIERCE + 123));
}

bool test_an_armor_added_to_255_is_one_change(void)
{
    DatChange armor = { ADD, NONE, INFANTRY, ARMOR, (float)(PIERCE + 85) };

    return bonus_scale(&armor, THRICE).count == 1;
}
