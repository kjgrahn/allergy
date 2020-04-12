/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_ENTITY_ENTITY_H_
#define GB_ENTITY_ENTITY_H_

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
     * The inclusion of 'status_code' makes this slightly more than
     * an entity, but most simply say "200 OK"; an obvious exception
     * being 404 pages.
     */
    struct Entity {
	Entity() = default;
	Entity(const Entity&) = delete;
	Entity& operator= (const Entity&) = delete;
	Entity& operator= (Entity&&) = default;

	static constexpr const char* status_code = "200 OK\r\n";
    };
}

#endif
