/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_ENTITY_H_
#define GB_ENTITY_ENTITY_H_

class Content;

namespace entity {

    /**
     * Basic entity stuff (but not the entire interface of entities;
     * no runtime polymorphism here).
     *
     * Entities represent the HTTP entities we care about:
     * - JPEG images
     * - 404 texts
     * - style sheets
     * - generated HTML indices
     * and so on.
     *
     * An entity can stream its entity headers [7.1] and feed its
     * entity-body through a filter using tick() and done().
     *
     * An entity also has const access to the Content, i.e. the
     * overall, static server state. It's used for things like the
     * Last-Modified: time for entities which have no natural such
     * time.
     */
    struct Entity {
	explicit Entity(const Content& content) : content{content} {}
	Entity(const Entity&) = delete;
	Entity& operator= (const Entity&) = delete;
	Entity& operator= (Entity&&) = default;

    private:
	const Content& content;
    };
}

#endif
