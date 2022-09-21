# Contribution Guidelines

First off, thank you for taking the time to contribute!🎉💪

Following these guidelines helps to communicate that you respect the time of the developers managing and developing this open source project. In return, they should reciprocate that respect in addressing your issue, assessing changes, and helping you finalize your pull requests.

## Contributing to development

amp-hal-st is a young open source project and we love to receive contributions from our community — you! There are many ways to contribute, from writing examples, improving the documentation, submitting bug reports and feature requests or writing code which can be incorporated into EmbeddedInfraLib itself.

## Ground Rules

* Be professional, respectful and considerate in your communication with others.
* As EmbeddedInfraLib core-team we have spent considerate effort to make our components testable; please make sure that you have test coverage for your changes as well. Inspiration can be drawn from existing components.
* One of the unique selling points of EmbeddedInfraLib is not using any heap; don't use the heap in your changes. This excludes using most components from the standard template library, for which we provide heapless alternatives.
* Ensure cross-platform compatibility for your changes.
* Adhere to the [Coding Standard C++ Embedded Projects]; when in doubt, look at the surrounding code that you are changing and stick to the local style.
* Create issues for any major changes and enhancements that you wish to make. Discuss things transparently and get core-team feedback.

## Your First Contribution

### Introduction

Submitting your first contribution (or Pull Request) can be scary, but we promise you it will be very rewarding! Information on how to proceed can be found on numerous blog posts and websites. A small selection of beginner friendly tutorials:

**Working on your first Pull Request?** You can learn how from this *free* series [How to Contribute to an Open Source Project on GitHub](https://app.egghead.io/playlists/how-to-contribute-to-an-open-source-project-on-github)

**Contributing to open source for the first time can be scary and a little overwhelming.** Perhaps you’re a [Code Newbie](https://www.codenewbie.org/) or maybe you’ve been coding for a while but haven’t found a project you felt comfortable contributing to. [You can do it! here's how.](https://www.firsttimersonly.com/)

#### Creating Pull Requests
1. Push your commit to get it back up to your fork: git push origin HEAD
2. Visit Github, click handy “Pull request” button that it will make upon noticing your new branch.
3. In the description field, write down issue number (if submitting code fixing an existing issue) or describe the issue + your fix (if submitting a wholly new bugfix).
4. Hit ‘submit’! And please be patient - the maintainers will get to you when they can.

## How to report a bug

If you find a security vulnerability, do NOT open an issue. Email one of the [maintainers](.github/CODEOWNERS) instead.

In order to determine whether you are dealing with a security issue, ask yourself these two questions:
* Can I access something that's not mine, or something I shouldn't have access to?
* Can I disable something for other people?

If the answer to either of those two questions are "yes", then you're probably dealing with a security issue. Note that even if you answer "no" to both questions, you may still be dealing with a security issue, so if you're unsure, just email us.

When filing an issue, make sure to answer these five questions:

1. What compiler and compiler version are you using?
2. What operating system and processor architecture are you using?
3. What did you do?
4. What did you expect to see?
5. What did you see instead?
