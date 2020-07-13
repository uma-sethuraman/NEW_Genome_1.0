#!/bin/bash

valgrind --tool=memcheck --leak-check=full ./test
