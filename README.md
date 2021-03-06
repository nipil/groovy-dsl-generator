# groovy-dsl-generator

a bison/flex tool to generate classes from a sample dsl

# requirements

## system

Debian Jessie / Ubuntu 16.04

    sudo apt-get install autoconf automake make g++ flex bison

CentOS 7

    sudo yum install autoconf automake make gcc-c++ flex bison

## groovy

*WARNING*: groovy version 2.3.0+ is required for the generated code.

If your distribution does not carry a proper version, have a look at [SDKMAN](http://sdkman.io) to get any version easily and securely.

# build & install

To install `gdg` in `/usr/local/bin`

    autoreconf --install
    ./configure
    make
    sudo make install

To install somewhere else, use `--prefix=/path/to` option of `./configure`

If you do not want to install via autotools, just copy `src/gdg` somewhere on your path (or use it in-place)

Then you can clean the build and intermediate files using `git clean -fdx`

# use

Edit a definition file, or use one in `samples-working`

Generate groovy classes and test dsl file :

    gdg path/to/definition/file

Then use the test command provided at the end of generation to run the DSL classes against the DSL test file

# example

One of the provided definition file :

    $ cat samples-working/0003.def

    @org.example.dsl.bank

    %person {
        age num
        name txt
        surname txt
    }

    %transaction {
        sender %person
        receiver %person
    }

    %statistics {
        balance num, %person
        accountInfo num, %person, num
    }

    account txt, %statistics

    manager %person

Generate groovy DSL code, and sample test file :

    $ src/gdg samples-working/0003.def

    Target package : org.example.dsl.bank
    Parsed 3 closure declaration: person statistics transaction
    Parsed 2 top-level DSL keywords: account(txt,statistics) manager(person)
    Creating folder output/src/main/groovy/org/example/dsl/bank
    Generating class PersonSpec for spec person
    Generating class StatisticsSpec for spec statistics
    Generating class TransactionSpec for spec transaction
    Generating class MasterScriptSpec for spec masterScript
    Generating class DelegateTrait for closure delegation
    Generating sample dsl file example.dsl
    To parse DSL: groovy -cp output/src/main/groovy -b org.example.dsl.bank.MasterScriptSpec output/example.dsl

Generated output is in `output` directory :

    $ find output -type f |sort
    output/example.dsl
    output/src/main/groovy/org/example/dsl/bank/DelegateTrait.groovy
    output/src/main/groovy/org/example/dsl/bank/MasterScriptSpec.groovy
    output/src/main/groovy/org/example/dsl/bank/PersonSpec.groovy
    output/src/main/groovy/org/example/dsl/bank/StatisticsSpec.groovy
    output/src/main/groovy/org/example/dsl/bank/TransactionSpec.groovy

The generated sample DSL file is `example.dsl` :

    $ cat output/example.dsl

    account "krbrtgnggmh", {
        balance 480, {
            age 629
            name "oklfpctwhx"
            surname "xvnioishjlbryhxlqc"
        }
        accountInfo 754, {
            age 17
            name "ewxtysphpdgmqody"
            surname "kjtnasw"
        }, 70
    }
    manager {
        age 41
        name "exxwbtvxunobcskoky"
        surname "lheucrxuozzcdya"
    }

Groovy DSL run :

    $ groovy -cp output/src/main/groovy -b org.example.dsl.bank.MasterScriptSpec output/example.dsl

    account: txt1=krbrtgnggmh
    balance: num1=480
    age: num1=629
    name: txt1=oklfpctwhx
    surname: txt1=xvnioishjlbryhxlqc
    accountInfo: num1=754
    age: num1=17
    name: txt1=ewxtysphpdgmqody
    surname: txt1=kjtnasw
    accountInfo: num3=70
    age: num1=41
    name: txt1=exxwbtvxunobcskoky
    surname: txt1=lheucrxuozzcdya

# definition syntax

See `samples-*` for appropriate (or forbidden syntax)

`@org.example.pwet` defines the package the classes belong to, to `org.example.pwet`. MUST be first definition in file.

`%typename { ... }` defines a custom type, ie a class `Typename` and its content `...`. This is used to include closure in later weyword definitions

`keyword type, type, type...` defines a keyword at the appropriate level (inside a custom closure or at top-level) and `...` are its parameter types

Types can be :

- `num` (mapped to an `int in groovy code)
- `txt` (mapped to a `String` in groovy code)
- `%typename` (mapped to a closure, delegated to an object of type `Typename`)

Refer to samples for pragmatic examples.

# non-working syntax

## examples

The samples in `samples-broken` are provided to show what does not work and why (explained as per error messages)

## case of `closure-with-*.def`

These files produce "_cannot have multiple custom types on same definition without re-introducing parenthesis in target dsl text_" error message.

In fact, these definitions could work. But Groovy parenthesis simplification make it so that:

For example :

    // works (closure second)
    a 1, {
        b 1
    }

    // works (closure first with parenthesis)
    a({
        b 1
    }, 1)

    // Groovy exception (closure first without parenthesis)
    a {
        b 1
    }, 1


While this would not (exception generated by Groovy on runtime)

    // works (double closure with parenthesis)
    a({
        b 1
    }, {
        b 1
    })

    // exception (double closure without parenthesis)
    a {
        b 1
    }, {
        b 1
    }

TL;DR : it seems you cannot have a closure as *first* argument *and* have somethin behind it *without* adding parenthesis back into to the DSL language. That's why i blocked it, as we want the simplest DSL possible.

Just for the sake of completeness, here is the explanation provided by the nice people on the groovy user mailing list : http://mail-archives.apache.org/mod_mbox/groovy-users/201611.mbox/%3C5839727A.8080603%40gmx.org%3E
