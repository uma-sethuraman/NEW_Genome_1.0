/**
 * \file AbstractHandler.h
 * \author Victoria Cao
 * 
 * \brief Abstract class for a handler of a given genome
 **/
#pragma once
#include <iostream>
#include <cstddef>

typedef char Byte;

/** Handler for traversing and mutating over genome **/
class AbstractHandler
{
protected:
    size_t Index = 0; ///< index into gene/site

public:
    /** (deleted) default Constructor **/
    AbstractHandler() = default;

    /** Deconstructor **/
    virtual ~AbstractHandler() {}

    /** Gets current position
    * \return position in segment **/
    const size_t GetIndex() { return Index; }

    /** Get value at current position
     * \returns Value at Pos in the collection */
    virtual const Byte Data() const
    {
        std::cout << "* operator has not been written for this Handler class" << std::endl;
        exit(1);
    }


    /** Resets handler to first index**/
    virtual void Reset()
    {
        std::cout << "Reset has not been written for this Handler class" << std::endl;
        exit(1);
    }

    /** Moves handler to next index*/
    virtual void Next()
    {
        std::cout << "Next has not been written for this Handler class" << std::endl;
        exit(1);
    }

    /** Moves handler to next index
     * \return if handler can move to next index**/
    virtual bool HasNext()
    {
        std::cout << "Next has not been written for this Handler class" << std::endl;
        exit(1);
    }


    /** Moves handler to previous index **/
    virtual void Prev()
    {
        std::cout << "Prev has not been written for this Handler class" << std::endl;
        exit(1);
    }

    /** Moves handler to previous index 
     * \return if handler can move to previous index**/
    virtual bool HasPrev()
    {
        std::cout << "Prev has not been written for this Handler class" << std::endl;
        exit(1);
    }


    /** Moves handler to index
     * \param index index to move to **/
    virtual void MoveTo(size_t index)
    {
        std::cout << "MoveTo has not been written for this Handler class" << std::endl;
        exit(1);
    }

    /** Makes a delete mutation at current index **/
    virtual void DeleteMutation(size_t index)
    {
        std::cout << "DeleteMutation has not been written for this Handler class" << std::endl;
        exit(1);
    }


    /** Makes a point mutation at current index
     * \param mutation point mutation to make**/
    virtual void PointMutation(size_t index, Byte* mutation, size_t size)
    {
        std::cout << "PointMutation has not been written for this Handler class" << std::endl;
        exit(1);
    }

    /** Makes a point mutation at current index
     * \param mutation point mutation to make**/

    virtual void InsertMutation(size_t index, Byte* mutation, size_t size)
    {
        std::cout << "InsertMutation has not been written for this Handler class" << std::endl;
        exit(1);
    }



    /** Prints handler**/
    virtual void Print()
    {
        std::cout << "Print has not been written for this Handler class" << std::endl;
        exit(1);
    }

};