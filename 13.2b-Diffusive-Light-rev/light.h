#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

/**
    Classe di supporto per gestire le informazion di luce ambientale
*/
class AmbientLight {
    glm::vec3 _color; ///<< Colore della luce
    float _intensity; ///<< Intensità della luce

public:
    /**
        Setta la luce al colore bianco e massima intensità
    */
    AmbientLight();
    /**
        Setta la luce al colore e intensità dati
        @param col colore dela luce
        @param i intensità della luce
    */
    AmbientLight(const glm::vec3 &col, float i);

    /**
        Incrementa l'intensità della luce della quantità data
        @param value valore di incremento
    */
    void inc(float value);

    /**
        Decrementa l'intensità della luce della quantità data
        @param value valore di decremento
    */
    void dec(float value);

    /**
        Ritorna il colore della luce
    */
    glm::vec3 color() const;

    /**
        Ritorna l'intensità della luce
    */
    float intensity() const;
};


class DiffusiveLight {
    glm::vec3 _color; ///<< Colore della luce
    glm::vec3 _direction; ///<< Direzione della sorgente
    float _intensity; /// Intensità della luce

public:
    /**
        Setta la luce al colore bianco e massima intensità e posizionata in alto
        La direzione di irraggiamento di default è un vettore verticale verso il basso.
    */
    DiffusiveLight();

    /**
        Setta la luce al colore e intensità dati
        @param col colore dela luce
        @param i intensità della luce
    */
    DiffusiveLight(const glm::vec3 &col, const glm::vec3 &dir, float i);
    /**
        Incrementa l'intensità della luce della quantità data
        @param value valore di incremento
    */
    void inc(float value);

    /**
        Decrementa l'intensità della luce della quantità data
        @param value valore di decremento
    */
    void dec(float value);

    /**
        Ritorna il colore della luce
    */
    glm::vec3 color() const;

    /**
        Ritorna la direzione della luce
    */
    glm::vec3 direction() const;

    /**
        Ritorna l'intensità della luce
    */
    float intensity() const;};


#endif