#ifndef PYLC3_HPP
#define PYLC3_HPP

#include <lc3_all.hpp>

/** A simple wrapper around lc3_state struct to bind to python. */
class LC3State
{
public:
    LC3State() { lc3_init(state); };
    /** @see lc3_init */
    void init(bool randomize_registers = true, bool randomize_memory = true, short register_fill_value = 0, short memory_fill_value = 0)
    {
        lc3_init(state, randomize_registers, randomize_memory, register_fill_value, memory_fill_value);
    }
    /** @see lc3_assemble */
    bool load(const std::string& filename);
    /** @see lc3_run */
    void run(unsigned int num) { lc3_run(state, num); }
    /** @see lc3_step */
    void step() { lc3_step(state); }
    /** @see lc3_back */
    void back() { lc3_back(state); }
    /** @see lc3_rewind */
    void rewind() { lc3_rewind(state); }
    /** @see lc3_finish */
    void finish() { lc3_finish(state); }
    /** @see lc3_next_line */
    void next_line() { lc3_next_line(state); }
    /** @see lc3_prev_line */
    void previous_line() { lc3_prev_line(state); }
    /** @see lc3_mem_read */
    short memory_read(unsigned short address) { return lc3_mem_read(state, address); }
    /** @see lc3_mem_write */
    void memory_write(unsigned short address, short value) { lc3_mem_write(state, address, value); }
private:
    lc3_state state;
};

/** A simple wrapper around lc3_test_suite struct to bind to python. */
class LC3TestSuite
{
public:
    LC3TestSuite();
    /** loads an xml file */
    bool load(const std::string& xml_file);
    /** @see lc3_run_test_suite */
    void run(const std::string& assembly_file, int run_number) { lc3_run_test_suite(suite, assembly_file, -1, run_number); }
private:
    lc3_test_suite suite;
};

#endif
